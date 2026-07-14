/*=============================================================================
 * map_defs.h  --  JJK World Map Engine  (GBA/NDS-style Pokémon architecture)
 *
 * Tileset   : tileset_final_test_output.png
 * Tile size : 16 x 16 px
 * Sheet size: 512 x 512 px  →  32 columns × 32 rows of tiles
 *
 * Layer conventions (draw order):
 *   LAYER_GROUND    (0) – terrain, paths, water, floor tiles
 *   LAYER_FRINGE    (1) – walls, building bases, lower tree trunks
 *                        renders ABOVE player bottom half
 *   LAYER_OVERHEAD  (2) – rooftops, treetops, tower tips
 *                        renders ABOVE the entire player sprite
 *
 * Tile ID encoding:
 *   -1 / TILE_EMPTY  = transparent / skip
 *   ≥0               = linear index into 32-column sheet
 *                      col = id % SHEET_COLS
 *                      row = id / SHEET_COLS
 *=============================================================================*/

#ifndef MAP_DEFS_H
#define MAP_DEFS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ── Tileset geometry ───────────────────────────────────────────────────── */
#define TILE_SIZE       16          /* pixels per tile edge                  */
#define SHEET_COLS      32          /* tiles per row in the PNG sheet        */
#define SHEET_ROWS      32          /* tiles per column in the PNG sheet     */
#define TILE_EMPTY      (-1)        /* sentinel: nothing to draw             */

/* ── Map layer indices ──────────────────────────────────────────────────── */
#define LAYER_COUNT     3
#define LAYER_GROUND    0           /* drawn first, always below player      */
#define LAYER_FRINGE    1           /* drawn after player lower body         */
#define LAYER_OVERHEAD  2           /* drawn last, always above player       */

/* ── Viewport / Camera ──────────────────────────────────────────────────── */
#define SCREEN_W        800         /* logical render resolution             */
#define SCREEN_H        600         /* landscape orientation window size     */
#define CAMERA_MARGIN   1           /* extra tile margin around screen edge  */

/* ── Collision flags (bitfield stored per ground tile) ──────────────────── */
#define COL_NONE        0x00
#define COL_SOLID       0x01        /* player cannot walk here               */
#define COL_WATER       0x02        /* needs Surf HM                         */
#define COL_LEDGE_S     0x04        /* one-way jump ledge heading south      */
#define COL_TRIGGER     0x08        /* script / warp trigger                 */

/* ── Tile IDs (named constants – see tileset layout below) ──────────────── */
/*
 *  Sheet row 0  (y=0):   Ground variants
 *  Sheet row 1  (y=1):   Building lower halves, doors
 *  Sheet row 2  (y=2):   Building upper halves
 *  Sheet row 3  (y=3):   Tall structures (Tokyo Tower, Sky Tree, gates …)
 *  Sheet row 4  (y=4):   Shinkansen train, buses, vehicles
 *  Sheet row 5  (y=5):   Props  (vending machines, bikes, cherry blossoms …)
 *  Sheet row 6  (y=6):   More props (phone box, post, shipping container …)
 *
 *  Linear ID = row * SHEET_COLS + col
 */

/* Ground row (row 0) */
#define TILE_GRASS          0       /* col 0, row 0 */
#define TILE_GRASS_DARK     1
#define TILE_DIRT_PATH      2
#define TILE_STONE_PATH     3
#define TILE_WATER          4
#define TILE_WATER_EDGE     5
#define TILE_SIDEWALK       6
#define TILE_ROAD           7
#define TILE_ROAD_MARK      8
#define TILE_SAND           9

/* Building bases – row 1 */
#define TILE_BLDG_RED_L     32      /* red-trim shop, left  col */
#define TILE_BLDG_RED_R     33      /* red-trim shop, right col */
#define TILE_BLDG_GLASS_L   34
#define TILE_BLDG_GLASS_R   35
#define TILE_POLICE_BOX     38
#define TILE_DOOR_WOOD      39
#define TILE_BLDG_BLUE_L    40
#define TILE_BLDG_BLUE_R    41
#define TILE_AWNING_RED     42
#define TILE_TREE_LOWER     43      /* tree bottom half                      */

/* Building tops – row 2 (overhead layer) */
#define TILE_BLDG_ROOF_L    64
#define TILE_BLDG_ROOF_R    65
#define TILE_TREE_UPPER     67      /* tree canopy (renders above player)    */

/* Tall structures – rows 3–4 (multi-tile, see multi_tile_defs.h) */
#define TILE_TOKYOTOWER_BASE    96
#define TILE_SKYTREE_BASE       98
#define TILE_TORII_BASE         100
#define TILE_PAGODA5_BASE       103

/* Vehicles – row 4 */
#define TILE_SHINKANSEN_L   128
#define TILE_SHINKANSEN_R   129
#define TILE_BUS_L          132
#define TILE_BUS_R          133

/* Props – row 5 */
#define TILE_VENDING_A      160
#define TILE_VENDING_B      161
#define TILE_BIKE_RACK      162
#define TILE_CHERRY_BLOSSOM 163
#define TILE_TRASH_CAN      167

/* Props – row 6 */
#define TILE_PHONE_BOX      192
#define TILE_POSTBOX        193
#define TILE_BENCH          194
#define TILE_CONTAINER_L    197
#define TILE_CONTAINER_R    198

/* ── Core data structures ───────────────────────────────────────────────── */

/** Single cell in a layer grid. */
typedef struct {
    int16_t  id;        /**< Tile index into sheet (-1 = empty/transparent)  */
    uint8_t  flags;     /**< Collision / trigger bitmask (LAYER_GROUND only) */
    uint8_t  _pad;
} Tile;

/** One full z-layer of the map. */
typedef struct {
    int      width;     /**< Map width  in tiles                             */
    int      height;    /**< Map height in tiles                             */
    Tile    *cells;     /**< Flat array [height * width], row-major          */
} MapLayer;

/** Complete map: three stacked layers + metadata. */
typedef struct {
    int       width;               /**< Width  in tiles                      */
    int       height;              /**< Height in tiles                       */
    MapLayer  layers[LAYER_COUNT]; /**< [GROUND, FRINGE, OVERHEAD]           */
    /* Future expansion: warp list, NPC spawn list, music track ID … */
} GameMap;

/* Raylib already defines Camera2D in raylib.h. Do not redefine it here. */

/* ── Helper: tile ID → sheet source rectangle ───────────────────────────── */
/** Fill *srcX and *srcY with the top-left pixel of tile `id` on the sheet. */
static inline void TileIDToSheetCoords(int id, int *srcX, int *srcY)
{
    *srcX = (id % SHEET_COLS) * TILE_SIZE;
    *srcY = (id / SHEET_COLS) * TILE_SIZE;
}

/** Access a tile in a layer by (col, row). Returns NULL if out of bounds. */
static inline Tile* LayerGetTile(MapLayer *layer, int col, int row)
{
    if (col < 0 || row < 0 || col >= layer->width || row >= layer->height)
        return NULL;
    return &layer->cells[row * layer->width + col];
}

#endif /* MAP_DEFS_H */
