/*=============================================================================
 * main.c  --  JJK World Map Engine  (Raylib entry point + game loop)
 *
 * Compilation (GCC + Raylib, Windows):
 *   gcc main.c map_engine.c -o jjk_game.exe -lraylib -lopengl32 -lgdi32 -lwinmm -lm
 *
 * Compilation (Clang + Raylib, Linux/macOS):
 *   clang main.c map_engine.c -o jjk_game -lraylib -lm
 *
 * Controls:
 *   Arrow keys / WASD  – move the "player" dot
 *   + / -              – zoom in / out
 *   ESC                – quit
 *=============================================================================*/

#include "raylib.h"
#include "map_defs.h"
#include "map_engine.h"
#include "sample_map.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ── Window & display settings ──────────────────────────────────────────── */
#define WINDOW_W     SCREEN_W
#define WINDOW_H     SCREEN_H
#define WINDOW_TITLE "JJK World – Pokémon-style Map Engine"
#define TARGET_FPS   60

/* ── Player constants ───────────────────────────────────────────────────── */
#define PLAYER_SPEED       80.0f    /* pixels per second                     */
#define PLAYER_SIZE        12       /* visual radius for placeholder dot     */
#define PLAYER_HITBOX_HALF  6.0f    /* half-size for collision AABB          */

/* ── Camera smoothing ───────────────────────────────────────────────────── */
#define CAM_SMOOTHING      0.15f    /* 0.0=frozen … 1.0=instant snap         */
#define CAM_ZOOM_MIN       1.0f
#define CAM_ZOOM_MAX       1.0f
#define CAM_ZOOM_STEP      0.25f

/* ══════════════════════════════════════════════════════════════════════════
 * Player state (simple struct – replace with your full entity system)
 * ══════════════════════════════════════════════════════════════════════════ */
typedef struct {
    float x;           /* World pixel position (top-left of sprite)         */
    float y;
    float velX;
    float velY;
} Player;

/* ── Movement & collision ─────────────────────────────────────────────────── */

/**
 * Attempt to move the player by (dx, dy).
 * Performs separate X and Y axis checks (gives smooth sliding along walls).
 */
static void PlayerMove(Player *player, const GameMap *map, float dx, float dy)
{
    /* Test X axis */
    float newX = player->x + dx;
    float testCX = (dx > 0) ? (newX + PLAYER_HITBOX_HALF * 2.0f) : newX;
    float testCY = player->y + PLAYER_HITBOX_HALF;

    uint8_t flagsX = MapGetCollisionAt(map, testCX, testCY);
    if (!(flagsX & COL_SOLID))
        player->x = newX;

    /* Test Y axis */
    float newY = player->y + dy;
    float testRX = player->x + PLAYER_HITBOX_HALF;
    float testRY = (dy > 0) ? (newY + PLAYER_HITBOX_HALF * 2.0f) : newY;

    uint8_t flagsY = MapGetCollisionAt(map, testRX, testRY);
    if (!(flagsY & COL_SOLID))
        player->y = newY;
}

/* ══════════════════════════════════════════════════════════════════════════
 * WorldToScreen helpers exposed for main.c
 * (map_engine.c has internal static versions; we define local versions here
 *  to avoid modifying the public API header.)
 * ══════════════════════════════════════════════════════════════════════════ */
static inline int WorldToScreenX_Pub(float worldX, Camera2D cam)
{
    return (int)roundf((worldX - cam.target.x) * cam.zoom + cam.offset.x);
}
static inline int WorldToScreenY_Pub(float worldY, Camera2D cam)
{
    return (int)roundf((worldY - cam.target.y) * cam.zoom + cam.offset.y);
}

/* ── Placeholder player sprite drawing (replace with your sprite sheet) ── */
static void DrawPlayer(Player *player, Camera2D cam)
{
    int scrX = WorldToScreenX_Pub(player->x + PLAYER_HITBOX_HALF, cam);
    int scrY = WorldToScreenY_Pub(player->y + PLAYER_HITBOX_HALF, cam);

    /* Shadow */
    DrawEllipse(scrX, scrY + PLAYER_SIZE - 2,
                PLAYER_SIZE / 2, PLAYER_SIZE / 5,
                (Color){0, 0, 0, 90});
    /* Body */
    DrawCircle(scrX, scrY, (float)PLAYER_SIZE, (Color){255, 220, 50, 255});
    /* Outline */
    DrawCircleLines(scrX, scrY, (float)PLAYER_SIZE, (Color){180, 140, 20, 255});
}



/* ══════════════════════════════════════════════════════════════════════════
 * Tileset loading helper
 *
 * Raylib loads PNG with alpha intact if it was exported with alpha channel.
 * If your tileset uses magenta (#FF00FF) masking without a true alpha,
 * we use ImageColorReplace to convert magenta → transparent before upload.
 * ══════════════════════════════════════════════════════════════════════════ */
static bool IsMagentaPixel(unsigned char r, unsigned char g, unsigned char b)
{
    return (r > 240 && g < 16 && b > 240);
}

static void ConvertMagentaToAlpha(Image *img)
{
    unsigned char *pixels = (unsigned char *)img->data;
    int pixelCount = img->width * img->height;

    for (int i = 0; i < pixelCount; i++) {
        unsigned char *px = pixels + i * 4;
        if (IsMagentaPixel(px[0], px[1], px[2])) {
            px[3] = 0;
        }
    }
}

static Texture2D LoadTilesetWithMagentaMask(const char *path)
{
    Image img = LoadImage(path);
    if (img.data == NULL) {
        TraceLog(LOG_ERROR, "Failed to load tileset: %s", path);
        return (Texture2D){0};
    }

    /* Convert image to RGBA so we can manipulate the alpha channel */
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    /* Replace exact magenta (#FF00FF) pixels with fully transparent */
    ConvertMagentaToAlpha(&img);

    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);

    /* Optional: nearest-neighbour filter preserves crisp pixel art */
    SetTextureFilter(tex, TEXTURE_FILTER_POINT);

    return tex;
}

/* ══════════════════════════════════════════════════════════════════════════
 * Main
 * ══════════════════════════════════════════════════════════════════════════ */
int main(void)
{
    /* ── Window init ──────────────────────────────────────────────────── */
    InitWindow(WINDOW_W, WINDOW_H, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);

    /* ── Load tileset ─────────────────────────────────────────────────── */
    Texture2D tileset = LoadTilesetWithMagentaMask("tileset_tokyo.png");
    if (tileset.id == 0) {
        CloseWindow();
        return 1;
    }

    /* ── Build sample map ─────────────────────────────────────────────── */
    GameMap map = {0};
    if (!SampleMap_Build(&map)) {
        TraceLog(LOG_ERROR, "Failed to allocate map layers");
        UnloadTexture(tileset);
        CloseWindow();
        return 1;
    }

    /* ── Initialise player at tile (3, 4) = centre of walkable grass ─── */
    /* Spawn at col 12, row 3 — inside Shibuya district walkable sidewalk */
    Player player = {
        .x    = 12 * TILE_SIZE + 4.0f,
        .y    =  3 * TILE_SIZE + 4.0f,
        .velX = 0.0f,
        .velY = 0.0f
    };

    /* ── Camera ───────────────────────────────────────────────────────── */
    Camera2D camera = {
        .offset   = { SCREEN_W * 0.5f, SCREEN_H * 0.5f },
        .target   = { player.x, player.y },
        .rotation = 0.0f,
        .zoom     = 1.0f          /* 1× pixel-perfect view */
    };

    /* ── Game loop ────────────────────────────────────────────────────── */
    while (!WindowShouldClose())
    {
        /* ── UPDATE ─────────────────────────────────────────────────── */
        float dt = GetFrameTime();

        /* Input → velocity */
        float moveX = 0.0f, moveY = 0.0f;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) moveX =  PLAYER_SPEED;
        if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) moveX = -PLAYER_SPEED;
        if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) moveY =  PLAYER_SPEED;
        if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) moveY = -PLAYER_SPEED;

        /* Normalise diagonal movement */
        if (moveX != 0.0f && moveY != 0.0f) {
            moveX *= 0.70711f;
            moveY *= 0.70711f;
        }

        PlayerMove(&player, &map, moveX * dt, moveY * dt);

        /* Camera follow + clamp */
        float centreX = player.x + PLAYER_HITBOX_HALF;
        float centreY = player.y + PLAYER_HITBOX_HALF;
        CameraFollow(&camera, centreX, centreY, CAM_SMOOTHING);
        CameraClamp(&camera, &map);

        /* ── DRAW ───────────────────────────────────────────────────── */
        BeginDrawing();
        ClearBackground((Color){30, 30, 40, 255});

        /*
         * Pokémon-style draw order:
         *   1. Ground layer   – terrain, roads
         *   2. Fringe layer   – building bases, tree trunks, vehicles
         *   3. Player sprite  – character between fringe and overhead
         *   4. Overhead layer – rooftops, treetops, tower tips
         */
        RenderMapBelow(&map, camera, tileset);
        DrawPlayer(&player, camera);
        RenderMapAbove(&map, camera, tileset);

        /* ── HUD ────────────────────────────────────────────────────── */
        DrawFPS(8, 8);

        int tileCol = (int)(player.x / TILE_SIZE);
        int tileRow = (int)(player.y / TILE_SIZE);
        uint8_t colFlag = MapGetCollisionFlags(&map, tileCol, tileRow);

        DrawText(TextFormat("Tile (%d, %d)  Col=0x%02X  Zoom=%.2fx",
                             tileCol, tileRow, colFlag, camera.zoom),
                 8, WINDOW_H - 22, 10, (Color){220, 220, 220, 200});

        EndDrawing();
    }

    /* ── Cleanup ──────────────────────────────────────────────────────── */
    MapFree(&map);
    UnloadTexture(tileset);
    CloseWindow();

    return 0;
}
