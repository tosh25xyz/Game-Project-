/*=============================================================================
 * map_engine.c  --  JJK World Map Engine  (implementation)
 *
 * Renderer backend : Raylib
 * Tile size        : 32 × 32 px
 * Sheet layout     : 16 columns wide (512 px sheet ÷ 32)
 *
 * Swap DrawTile_Raylib() for an SDL2 equivalent if needed — all other
 * logic (culling, layer ordering, camera math) is backend-agnostic.
 *=============================================================================*/

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "raylib.h"       /* DrawTextureRec, Rectangle, Color, Texture2D … */
#include "map_defs.h"
#include "map_engine.h"

/* ══════════════════════════════════════════════════════════════════════════
 * Internal helpers
 * ══════════════════════════════════════════════════════════════════════════ */

/**
 * Draw a single 32×32 tile from the sheet to the screen.
 *
 * @param tileset   Loaded Raylib Texture2D of the PNG sheet
 * @param tileID    Linear tile index (< 0 → skip)
 * @param screenX   Destination X in screen-space (already camera-transformed)
 * @param screenY   Destination Y in screen-space
 */
static void DrawTile_Raylib(Texture2D tileset, int tileID,
                             int screenX, int screenY)
{
    if (tileID < 0) return;         /* TILE_EMPTY – nothing to draw          */

    int srcX, srcY;
    TileIDToSheetCoords(tileID, &srcX, &srcY);

    Rectangle src  = { (float)srcX, (float)srcY,
                        (float)TILE_SIZE, (float)TILE_SIZE };
    Vector2   dest = { (float)screenX, (float)screenY };

    /* WHITE tint = no colour modification; Raylib handles magenta mask via
       the texture's alpha channel.  Load the PNG with GenTextureMipmaps or
       simply load via LoadTexture — magenta should be pre-converted to alpha
       in your asset pipeline.  If not, use ImageColorReplace() at load time. */
    DrawTextureRec(tileset, src, dest, WHITE);
}

/* ── Viewport culling helpers ────────────────────────────────────────────── */

/**
 * Compute the range of tile columns and rows visible through the camera.
 * Results are clamped to the map boundaries.
 *
 * @param camera    Current camera state
 * @param mapW      Map width  in tiles
 * @param mapH      Map height in tiles
 * @param colMin    [out] First visible column
 * @param colMax    [out] Last  visible column (inclusive)
 * @param rowMin    [out] First visible row
 * @param rowMax    [out] Last  visible row (inclusive)
 */
static void ComputeVisibleTileRange(Camera2D camera,
                                    int mapW, int mapH,
                                    int *colMin, int *colMax,
                                    int *rowMin, int *rowMax)
{
    /* Half-screen size in world pixels (accounting for zoom) */
    float halfW = (SCREEN_W * 0.5f) / camera.zoom;
    float halfH = (SCREEN_H * 0.5f) / camera.zoom;

    /* World-space pixel rectangle of the viewport */
    float vpLeft   = camera.target.x - halfW;
    float vpTop    = camera.target.y - halfH;
    float vpRight  = camera.target.x + halfW;
    float vpBottom = camera.target.y + halfH;

    /* Convert to tile indices, add margin so border tiles aren't clipped */
    *colMin = (int)(vpLeft   / TILE_SIZE) - CAMERA_MARGIN;
    *rowMin = (int)(vpTop    / TILE_SIZE) - CAMERA_MARGIN;
    *colMax = (int)(vpRight  / TILE_SIZE) + CAMERA_MARGIN;
    *rowMax = (int)(vpBottom / TILE_SIZE) + CAMERA_MARGIN;

    /* Clamp to map extents */
    if (*colMin < 0)     *colMin = 0;
    if (*rowMin < 0)     *rowMin = 0;
    if (*colMax >= mapW) *colMax = mapW - 1;
    if (*rowMax >= mapH) *rowMax = mapH - 1;
}

/**
 * Convert a world-space pixel position to a screen-space position
 * given the current camera, ready for Raylib's DrawTextureRec.
 */
static inline int WorldToScreenX(float worldX, Camera2D camera)
{
    return (int)roundf((worldX - camera.target.x) * camera.zoom + camera.offset.x);
}

static inline int WorldToScreenY(float worldY, Camera2D camera)
{
    return (int)roundf((worldY - camera.target.y) * camera.zoom + camera.offset.y);
}

/* ══════════════════════════════════════════════════════════════════════════
 * Map lifecycle
 * ══════════════════════════════════════════════════════════════════════════ */

bool MapAllocLayers(GameMap *map)
{
    size_t cellCount = (size_t)map->width * (size_t)map->height;

    for (int li = 0; li < LAYER_COUNT; li++) {
        map->layers[li].width  = map->width;
        map->layers[li].height = map->height;
        map->layers[li].cells  = (Tile *)calloc(cellCount, sizeof(Tile));

        if (!map->layers[li].cells) {
            /* Roll back already-allocated layers */
            for (int prev = 0; prev < li; prev++) {
                free(map->layers[prev].cells);
                map->layers[prev].cells = NULL;
            }
            return false;
        }

        /* Initialise all tile IDs to TILE_EMPTY */
        for (size_t i = 0; i < cellCount; i++)
            map->layers[li].cells[i].id = TILE_EMPTY;
    }
    return true;
}

void MapLayerLoadFromArray(GameMap *map, int layerIndex,
                           const int16_t *data,
                           const uint8_t *collisionData)
{
    MapLayer *layer    = &map->layers[layerIndex];
    size_t    total    = (size_t)layer->width * (size_t)layer->height;

    for (size_t i = 0; i < total; i++) {
        layer->cells[i].id    = data[i];
        layer->cells[i].flags = collisionData ? collisionData[i] : COL_NONE;
    }
}

void MapFree(GameMap *map)
{
    for (int li = 0; li < LAYER_COUNT; li++) {
        free(map->layers[li].cells);
        map->layers[li].cells = NULL;
    }
}

/* ══════════════════════════════════════════════════════════════════════════
 * Camera
 * ══════════════════════════════════════════════════════════════════════════ */

void CameraFollow(Camera2D *camera, float playerWorldX, float playerWorldY,
                  float smoothing)
{
    /* Lerp camera target toward player position */
    camera->target.x += (playerWorldX - camera->target.x) * smoothing;
    camera->target.y += (playerWorldY - camera->target.y) * smoothing;
}

void CameraClamp(Camera2D *camera, const GameMap *map)
{
    float halfW = (SCREEN_W * 0.5f) / camera->zoom;
    float halfH = (SCREEN_H * 0.5f) / camera->zoom;

    float mapPixelW = (float)(map->width  * TILE_SIZE);
    float mapPixelH = (float)(map->height * TILE_SIZE);

    /* Don't let the camera show black bars on any side */
    if (camera->target.x - halfW < 0.0f)           camera->target.x = halfW;
    if (camera->target.y - halfH < 0.0f)           camera->target.y = halfH;
    if (camera->target.x + halfW > mapPixelW)      camera->target.x = mapPixelW - halfW;
    if (camera->target.y + halfH > mapPixelH)      camera->target.y = mapPixelH - halfH;
}

/* ══════════════════════════════════════════════════════════════════════════
 * Rendering
 * ══════════════════════════════════════════════════════════════════════════ */

void RenderLayer(const GameMap *map, int layerIndex,
                 Camera2D camera, Texture2D tileset)
{
    const MapLayer *layer = &map->layers[layerIndex];

    int colMin, colMax, rowMin, rowMax;
    ComputeVisibleTileRange(camera,
                            map->width, map->height,
                            &colMin, &colMax,
                            &rowMin, &rowMax);

    for (int row = rowMin; row <= rowMax; row++) {
        for (int col = colMin; col <= colMax; col++) {

            const Tile *tile = &layer->cells[row * layer->width + col];
            if (tile->id < 0) continue;     /* TILE_EMPTY → skip             */

            /* World-space top-left corner of this tile (pixels) */
            float worldX = (float)(col * TILE_SIZE);
            float worldY = (float)(row * TILE_SIZE);

            /* Transform to screen space */
            int scrX = WorldToScreenX(worldX, camera);
            int scrY = WorldToScreenY(worldY, camera);

            DrawTile_Raylib(tileset, tile->id, scrX, scrY);
        }
    }
}

/* ── Layered draw helpers ─────────────────────────────────────────────────── */

void RenderMapBelow(const GameMap *map, Camera2D camera, Texture2D tileset)
{
    RenderLayer(map, LAYER_GROUND,  camera, tileset);
    RenderLayer(map, LAYER_FRINGE,  camera, tileset);
}

void RenderMapAbove(const GameMap *map, Camera2D camera, Texture2D tileset)
{
    RenderLayer(map, LAYER_OVERHEAD, camera, tileset);
}

void RenderMap(const GameMap *map, Camera2D camera, Texture2D tileset)
{
    RenderMapBelow(map, camera, tileset);
    RenderMapAbove(map, camera, tileset);
}

/* ══════════════════════════════════════════════════════════════════════════
 * Collision
 * ══════════════════════════════════════════════════════════════════════════ */

uint8_t MapGetCollisionFlags(const GameMap *map, int col, int row)
{
    if (col < 0 || row < 0 || col >= map->width || row >= map->height)
        return COL_SOLID;           /* Out-of-bounds is always solid          */

    return map->layers[LAYER_GROUND].cells[row * map->width + col].flags;
}

uint8_t MapGetCollisionAt(const GameMap *map, float worldPixelX, float worldPixelY)
{
    int col = (int)(worldPixelX / TILE_SIZE);
    int row = (int)(worldPixelY / TILE_SIZE);
    return MapGetCollisionFlags(map, col, row);
}
