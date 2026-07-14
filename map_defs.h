/*=============================================================================
 * map_defs.h  --  JJK World Map Engine  (GBA/NDS-style Pokémon architecture)
 *
 * Tileset   : tileset_tokyo.png  (assembled from GK Japanese City Free Pack)
 * Tile size : 32 x 32 px
 * Sheet size: 512 x 768 px  →  16 columns × 24 rows of tiles (384 slots)
 * Source    : GK_JC_Free_001.png ... GK_JC_Free_379.png
 *             Tile ID = (file_number - 1)
 *             col = id % 16,   row = id / 16
 *
 * Layer conventions (draw order):
 *   LAYER_GROUND    (0) – terrain, pavement, roads
 *   LAYER_FRINGE    (1) – building bases, signs, props, lower walls
 *                        renders ABOVE player bottom half
 *   LAYER_OVERHEAD  (2) – rooftops, treetops
 *                        renders ABOVE the entire player sprite
 *
 * Tile ID encoding:
 *   -1 / TILE_EMPTY  = transparent / skip
 *   ≥0               = linear index into 16-column sheet
 *                      col = id % SHEET_COLS
 *                      row = id / SHEET_COLS
 *=============================================================================*/

#ifndef MAP_DEFS_H
#define MAP_DEFS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ── Tileset geometry ───────────────────────────────────────────────────── */
#define TILE_SIZE       32          /* pixels per tile edge (32×32 px)       */
#define SHEET_COLS      16          /* tiles per row in the PNG sheet        */
#define SHEET_ROWS      24          /* tiles per column in the PNG sheet     */
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

/*=============================================================================
 * TILE ID REFERENCE  (ID = file_number - 1, 0-indexed)
 *
 * GK Japanese City Free Pack — tile catalogue
 *
 * ── GROUND / PAVEMENT ─────────────────────────────────────────────────────
 *   Tile 184  (file 185): Sidewalk stone - plain mid checker A
 *   Tile 185  (file 186): Sidewalk stone - plain mid checker B
 *   Tile 186  (file 187): Sidewalk stone - plain mid checker C
 *   Tile 187  (file 188): Sidewalk stone - plain mid checker D
 *   Tile 188  (file 189): Sidewalk edge north curve
 *   Tile 189  (file 190): Sidewalk edge south curve
 *   Tile 190  (file 191): Sidewalk corner NW
 *   Tile 191  (file 192): Sidewalk corner NE
 *   Tile 192  (file 193): Sidewalk mid tile A (dark checkered)
 *   Tile 193  (file 194): Sidewalk mid tile B
 *   Tile 194  (file 195): Sidewalk mid tile C
 *   Tile 195  (file 196): Sidewalk mid tile D
 *   Tile 200  (file 201): Road asphalt - plain dark A
 *   Tile 201  (file 202): Road asphalt - plain dark B
 *   Tile 202  (file 203): Road asphalt - plain dark C
 *   Tile 203  (file 204): Road asphalt - plain dark D
 *   Tile 204  (file 205): Road asphalt - scuff mark
 *   Tile 209  (file 210): Road edge / kerb tile
 *   Tile 280  (file 281): Crosswalk stripe vertical A
 *   Tile 281  (file 282): Crosswalk stripe vertical B
 *   Tile 282  (file 283): Crosswalk stripe vertical C
 *   Tile 283  (file 284): Road centre white dashes
 *   Tile 359  (file 360): Water - dark river A
 *   Tile 360  (file 361): Water - dark river B
 *   Tile 361  (file 362): Water - dark river C
 *
 * ── BUILDING GROUND FLOOR (FRINGE) ────────────────────────────────────────
 *   Tile 82   (file 83):  Building wall dark horizontal A
 *   Tile 83   (file 84):  Building wall dark horizontal B
 *   Tile 84   (file 85):  Building wall brown stripe A
 *   Tile 85   (file 86):  Building wall brown stripe B
 *   Tile 86   (file 87):  Building wall red/orange accent
 *   Tile 87   (file 88):  Building wall with window (light)
 *   Tile 88   (file 89):  Street lamp post
 *   Tile 89   (file 90):  Street lamp with head
 *   Tile 90   (file 91):  Vending machine A (red)
 *   Tile 91   (file 92):  Vending machine B (blue)
 *   Tile 92   (file 93):  Vending machine C (white)
 *   Tile 93   (file 94):  ATM machine
 *   Tile 102  (file 103): Shop sliding door left
 *   Tile 103  (file 104): Shop sliding door mid
 *   Tile 104  (file 105): Shop sliding door right
 *   Tile 105  (file 106): Building entrance noren (curtain) left
 *   Tile 106  (file 107): Building entrance noren right
 *   Tile 309  (file 310): Torii gate pillar left
 *   Tile 310  (file 311): Torii gate pillar right
 *   Tile 311  (file 312): Torii gate crossbeam
 *   Tile 312  (file 313): Shrine lantern
 *   Tile 313  (file 314): Barrel / trash container
 *   Tile 314  (file 315): Trash can
 *   Tile 354  (file 355): Cherry blossom tree trunk
 *   Tile 355  (file 356): Cherry blossom single branch
 *   Tile 356  (file 357): Cherry blossom full canopy A
 *   Tile 357  (file 358): Cherry blossom full canopy B
 *   Tile 358  (file 359): Cherry blossom canopy C
 *
 * ── BUILDING UPPER (FRINGE / OVERHEAD) ───────────────────────────────────
 *   Tiles  0– 5: Roof edge NW corner + N edge pieces (dark tiled roof)
 *   Tiles  6–11: More roof edge tiles (mid/E/NE)
 *   Tiles 16–19: Roof peak / ridge tiles
 *   Tiles 20–35: Large building facade (multi-tile structure)
 *   Tiles 36–59: Extended building / 2F walls
 *
 * ── TRADITIONAL BUILDING (FRINGE) ─────────────────────────────────────────
 *   Tiles 320–339: Traditional roof tiles (pagoda style)
 *   Tiles 340–353: Traditional building wall / floor
 *
 *=============================================================================*/

/* ── Ground / Pavement tile IDs ─────────────────────────────────────────── */
#define TILE_SIDEWALK_A     184     /* plain stone checker A                 */
#define TILE_SIDEWALK_B     185     /* plain stone checker B                 */
#define TILE_SIDEWALK_C     186     /* plain stone checker C                 */
#define TILE_SIDEWALK_D     187     /* plain stone checker D                 */
#define TILE_SW_EDGE_N      188     /* sidewalk north edge curve             */
#define TILE_SW_EDGE_S      189     /* sidewalk south edge curve             */
#define TILE_SW_CORNER_NW   190     /* sidewalk corner NW                   */
#define TILE_SW_CORNER_NE   191     /* sidewalk corner NE                   */
#define TILE_SW_CHECK_A     192     /* dark checkered sidewalk A             */
#define TILE_SW_CHECK_B     193     /* dark checkered sidewalk B             */
#define TILE_SW_CHECK_C     194     /* dark checkered sidewalk C             */
#define TILE_SW_CHECK_D     195     /* dark checkered sidewalk D             */

#define TILE_ROAD_A         200     /* asphalt dark A                        */
#define TILE_ROAD_B         201     /* asphalt dark B                        */
#define TILE_ROAD_C         202     /* asphalt dark C                        */
#define TILE_ROAD_D         203     /* asphalt dark D                        */
#define TILE_ROAD_SCUFF     204     /* road scuff mark (variation)           */
#define TILE_ROAD_KERB      209     /* road edge kerb                        */

#define TILE_CROSSWALK_A    280     /* crosswalk stripe A                    */
#define TILE_CROSSWALK_B    281     /* crosswalk stripe B                    */
#define TILE_CROSSWALK_C    282     /* crosswalk stripe C                    */
#define TILE_ROAD_MARK      283     /* road centre dashes                    */

#define TILE_WATER_A        359     /* dark river water A                    */
#define TILE_WATER_B        360     /* dark river water B                    */
#define TILE_WATER_C        361     /* dark river water C                    */

/* ── Building wall / facade (fringe layer) ──────────────────────────────── */
#define TILE_WALL_DARK_A    82      /* dark horizontal stripe A              */
#define TILE_WALL_DARK_B    83      /* dark horizontal stripe B              */
#define TILE_WALL_BROWN_A   84      /* brown horizontal stripe A             */
#define TILE_WALL_BROWN_B   85      /* brown horizontal stripe B             */
#define TILE_WALL_ACCENT    86      /* red/orange accent stripe              */
#define TILE_WALL_WINDOW    87      /* wall with window                      */

/* ── Street furniture ───────────────────────────────────────────────────── */
#define TILE_LAMP_POST      88      /* street lamp post                      */
#define TILE_LAMP_HEAD      89      /* street lamp with head                 */
#define TILE_VENDING_R      90      /* vending machine red                   */
#define TILE_VENDING_B2     91      /* vending machine blue                  */
#define TILE_VENDING_W      92      /* vending machine white                 */
#define TILE_ATM            93      /* ATM machine                           */
#define TILE_TRASH_CAN      313     /* trash can                             */
#define TILE_BARREL         312     /* barrel / container                    */

/* ── Shop fronts ────────────────────────────────────────────────────────── */
#define TILE_SHOP_DOOR_L    102     /* sliding door left                     */
#define TILE_SHOP_DOOR_M    103     /* sliding door mid                      */
#define TILE_SHOP_DOOR_R    104     /* sliding door right                    */
#define TILE_NOREN_L        105     /* noren curtain left                    */
#define TILE_NOREN_R        106     /* noren curtain right                   */

/* ── Shrine / traditional ────────────────────────────────────────────────── */
#define TILE_TORII_L        309     /* torii pillar left                     */
#define TILE_TORII_R        310     /* torii pillar right                    */
#define TILE_TORII_BEAM     311     /* torii crossbeam                       */
#define TILE_SHRINE_LANTERN 312     /* shrine lantern                        */

/* ── Cherry blossom tree ─────────────────────────────────────────────────── */
#define TILE_SAKURA_TRUNK   354     /* trunk base                            */
#define TILE_SAKURA_BRANCH  355     /* single branch                         */
#define TILE_SAKURA_TOP_A   356     /* full canopy A (overhead)              */
#define TILE_SAKURA_TOP_B   357     /* full canopy B (overhead)              */
#define TILE_SAKURA_TOP_C   358     /* canopy C (overhead)                   */

/* ── Building rooftop (overhead) ────────────────────────────────────────── */
#define TILE_ROOF_EDGE_NW   0       /* roof corner NW                        */
#define TILE_ROOF_EDGE_N    1       /* roof north edge                       */
#define TILE_ROOF_EDGE_NE   2       /* roof corner NE                        */
#define TILE_ROOF_MID_A     3       /* roof mid A                            */
#define TILE_ROOF_MID_B     4       /* roof mid B                            */
#define TILE_ROOF_MID_C     5       /* roof mid C                            */
#define TILE_ROOF_PEAK_L    16      /* roof ridge left                       */
#define TILE_ROOF_PEAK_M    17      /* roof ridge mid                        */
#define TILE_ROOF_PEAK_R    18      /* roof ridge right                      */

/* ── Large building structure (fringe + overhead multi-tile) ─────────────── */
/* Row 1 (fringe - building 2F front row) */
#define TILE_BLDG_2F_L      20      /* 2F building facade left               */
#define TILE_BLDG_2F_M      21      /* 2F building facade mid                */
#define TILE_BLDG_2F_R      22      /* 2F building facade right              */
/* Row 2 (fringe - building 1F front row) */
#define TILE_BLDG_1F_L      36      /* 1F building facade left               */
#define TILE_BLDG_1F_M      37      /* 1F building facade mid                */
#define TILE_BLDG_1F_R      38      /* 1F building facade right              */

/* ── Traditional architecture ────────────────────────────────────────────── */
#define TILE_TRAD_ROOF_A    320     /* traditional roof tile A               */
#define TILE_TRAD_ROOF_B    321     /* traditional roof tile B               */
#define TILE_TRAD_ROOF_C    322     /* traditional roof tile C               */
#define TILE_TRAD_WALL_A    340     /* traditional wall A                    */
#define TILE_TRAD_WALL_B    341     /* traditional wall B                    */
#define TILE_TRAD_WALL_C    342     /* traditional wall C                    */

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
} GameMap;

/* ── Helper: tile ID → sheet source rectangle ───────────────────────────── */
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
