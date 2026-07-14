/*=============================================================================
 * sample_map.h  --  Hardcoded sample map: "JJK Tokyo Metropolis"
 *
 * Dimensions: 120 x 120 tiles  (each tile = 16 x 16 px → 1920 x 1920 px world)
 *
 * District layout (approximate):
 *
 *   Rows   0– 9  : Northern border – concrete wall + park strip
 *   Rows  10–29  : SHIBUYA WARD  – dense shopping blocks, crosswalks
 *   Rows  30–34  : Major E-W road corridor (4 lanes + road markings)
 *   Rows  35–54  : SHINJUKU WARD – skyscrapers, Tokyo Tower plaza
 *   Rows  55–59  : Rail / Shinkansen corridor (horizontal transit line)
 *   Rows  60–79  : AKIHABARA DISTRICT – tech shops, vending machine rows
 *   Rows  80–84  : Major E-W road corridor
 *   Rows  85–104 : UENO PARK DISTRICT – large grass fields, cherry blossoms
 *   Rows 105–114 : ASAKUSA WARD – torii gate, pagoda, temple area
 *   Rows 115–119 : Southern border – sidewalk perimeter
 *
 * Vertical corridors (N–S):
 *   Cols   0– 3  : West border sidewalk
 *   Cols   4– 9  : West road artery (N–S, 2 lanes)
 *   Cols  10–49  : West district blocks
 *   Cols  50–59  : Central N–S road artery (4 lanes)
 *   Cols  60–109 : East district blocks
 *   Cols 110–115 : East road artery (N–S, 2 lanes)
 *   Cols 116–119 : East border sidewalk
 *
 * Layer conventions (from map_defs.h):
 *   g_groundData   – terrain tile IDs
 *   g_groundFlags  – COL_SOLID flags per tile
 *   g_fringeData   – building bases, props, vehicles (renders above player feet)
 *   g_overheadData – rooftops, treetops (renders above entire player)
 *=============================================================================*/

#ifndef SAMPLE_MAP_HEADER
#define SAMPLE_MAP_HEADER

#include "map_defs.h"

/* ── Map dimensions ─────────────────────────────────────────────────────── */
#define SAMPLE_MAP_W   120
#define SAMPLE_MAP_H   120

/*
 * Shorthand macros for ground tile IDs (undefined at bottom of file)
 */
#define __   TILE_EMPTY
#define GR   TILE_GRASS
#define GD   TILE_GRASS_DARK
#define SW   TILE_SIDEWALK
#define RD   TILE_ROAD
#define RM   TILE_ROAD_MARK

/* Collision shorthands */
#define _O   COL_NONE
#define _S   COL_SOLID

/*===========================================================================
 * LAYER 0 – GROUND DATA
 * 120 rows × 120 columns of int16_t tile IDs.
 *
 * Road artery columns 4-9   (N-S west)
 * Road artery columns 50-59 (N-S central)
 * Road artery columns 110-115 (N-S east)
 * Road belt rows 30-34 (E-W major)
 * Rail belt rows 55-59
 * Road belt rows 80-84 (E-W major)
 *===========================================================================*/

/* Helper row macros – each expands to exactly 120 values */

/* BORDER SIDEWALK ROW (all SW) */
#define BORDER_SW_ROW \
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,\
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,\
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,\
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,\
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,\
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW

/* Full asphalt road row (E-W major corridor) */
#define ROAD_FULL_ROW \
    SW,SW,SW,SW,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,\
    RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,\
    RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,\
    RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,\
    RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,\
    RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,SW,SW,SW,SW,SW,SW

/* Road marking centre row */
#define ROAD_MARK_ROW \
    SW,SW,SW,SW,RM,RD,RD,RD,RD,RM,RD,RD,RM,RD,RD,RM,RD,RD,RM,RD,\
    RD,RM,RD,RD,RM,RD,RD,RM,RD,RD,RM,RD,RD,RM,RD,RD,RM,RD,RD,RM,\
    RD,RD,RM,RD,RD,RM,RD,RD,RM,RD,RM,RM,RM,RM,RM,RM,RM,RM,RM,RM,\
    RD,RD,RM,RD,RD,RM,RD,RD,RM,RD,RD,RM,RD,RD,RM,RD,RD,RM,RD,RD,\
    RM,RD,RD,RM,RD,RD,RM,RD,RD,RM,RD,RD,RM,RD,RD,RM,RD,RD,RD,RD,\
    RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RM,RD,RD,RD,SW,SW,SW,SW,SW,SW

/* Rail corridor row – road surface under trains */
#define RAIL_ROAD_ROW \
    SW,SW,SW,SW,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,\
    RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,\
    RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,\
    RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,\
    RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,\
    RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,RD,SW,SW,SW,SW,SW,SW

/* Generic district row: border SW | W road | district grass/SW | C road | district | E road | border SW */
/* c=col:  0  1  2  3 | 4..9 | 10..49 | 50..59 | 60..109 | 110..115 | 116..119 */
#define DIST_GR_ROW \
    SW,SW,SW,SW,RD,RD,RD,RD,RD,RD,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,\
    GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,\
    GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,RD,RD,RD,RD,RM,RM,RD,RD,RD,RD,\
    GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,\
    GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,\
    GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,RD,RD,RD,RD,SW,SW,SW,SW,SW,SW

#define DIST_SW_ROW \
    SW,SW,SW,SW,RD,RD,RD,RD,RD,RD,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,\
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,\
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,RD,RD,RD,RD,RM,RM,RD,RD,RD,RD,\
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,\
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,\
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,RD,RD,RD,RD,SW,SW,SW,SW,SW,SW

static const int16_t g_groundData[SAMPLE_MAP_H][SAMPLE_MAP_W] = {
/* r0  */ { BORDER_SW_ROW },
/* r1  */ { BORDER_SW_ROW },
/* r2  */ { DIST_SW_ROW   },
/* r3  */ { DIST_GR_ROW   },
/* r4  */ { DIST_GR_ROW   },
/* r5  */ { DIST_GR_ROW   },
/* r6  */ { DIST_SW_ROW   },
/* r7  */ { DIST_GR_ROW   },
/* r8  */ { DIST_GR_ROW   },
/* r9  */ { DIST_GR_ROW   },
/* r10 */ { DIST_SW_ROW   },
/* r11 */ { DIST_GR_ROW   },
/* r12 */ { DIST_GR_ROW   },
/* r13 */ { DIST_GR_ROW   },
/* r14 */ { DIST_SW_ROW   },
/* r15 */ { DIST_GR_ROW   },
/* r16 */ { DIST_GR_ROW   },
/* r17 */ { DIST_GR_ROW   },
/* r18 */ { DIST_SW_ROW   },
/* r19 */ { DIST_GR_ROW   },
/* r20 */ { DIST_GR_ROW   },
/* r21 */ { DIST_GR_ROW   },
/* r22 */ { DIST_SW_ROW   },
/* r23 */ { DIST_GR_ROW   },
/* r24 */ { DIST_GR_ROW   },
/* r25 */ { DIST_GR_ROW   },
/* r26 */ { DIST_SW_ROW   },
/* r27 */ { DIST_GR_ROW   },
/* r28 */ { DIST_GR_ROW   },
/* r29 */ { DIST_GR_ROW   },
/* r30 */ { ROAD_FULL_ROW },  /* E-W major road belt start */
/* r31 */ { ROAD_FULL_ROW },
/* r32 */ { ROAD_MARK_ROW },  /* centre dashes */
/* r33 */ { ROAD_FULL_ROW },
/* r34 */ { ROAD_FULL_ROW },  /* E-W major road belt end */
/* r35 */ { DIST_SW_ROW   },
/* r36 */ { DIST_GR_ROW   },
/* r37 */ { DIST_GR_ROW   },
/* r38 */ { DIST_GR_ROW   },
/* r39 */ { DIST_SW_ROW   },
/* r40 */ { DIST_GR_ROW   },
/* r41 */ { DIST_GR_ROW   },
/* r42 */ { DIST_GR_ROW   },
/* r43 */ { DIST_SW_ROW   },
/* r44 */ { DIST_GR_ROW   },
/* r45 */ { DIST_GR_ROW   },
/* r46 */ { DIST_GR_ROW   },
/* r47 */ { DIST_SW_ROW   },
/* r48 */ { DIST_GR_ROW   },
/* r49 */ { DIST_GR_ROW   },
/* r50 */ { DIST_GR_ROW   },
/* r51 */ { DIST_SW_ROW   },
/* r52 */ { DIST_GR_ROW   },
/* r53 */ { DIST_GR_ROW   },
/* r54 */ { DIST_GR_ROW   },
/* r55 */ { RAIL_ROAD_ROW },  /* Shinkansen rail belt start */
/* r56 */ { RAIL_ROAD_ROW },
/* r57 */ { RAIL_ROAD_ROW },
/* r58 */ { RAIL_ROAD_ROW },
/* r59 */ { RAIL_ROAD_ROW },  /* Shinkansen rail belt end */
/* r60 */ { DIST_SW_ROW   },
/* r61 */ { DIST_GR_ROW   },
/* r62 */ { DIST_GR_ROW   },
/* r63 */ { DIST_GR_ROW   },
/* r64 */ { DIST_SW_ROW   },
/* r65 */ { DIST_GR_ROW   },
/* r66 */ { DIST_GR_ROW   },
/* r67 */ { DIST_GR_ROW   },
/* r68 */ { DIST_SW_ROW   },
/* r69 */ { DIST_GR_ROW   },
/* r70 */ { DIST_GR_ROW   },
/* r71 */ { DIST_GR_ROW   },
/* r72 */ { DIST_SW_ROW   },
/* r73 */ { DIST_GR_ROW   },
/* r74 */ { DIST_GR_ROW   },
/* r75 */ { DIST_GR_ROW   },
/* r76 */ { DIST_SW_ROW   },
/* r77 */ { DIST_GR_ROW   },
/* r78 */ { DIST_GR_ROW   },
/* r79 */ { DIST_GR_ROW   },
/* r80 */ { ROAD_FULL_ROW },  /* E-W major road belt 2 start */
/* r81 */ { ROAD_FULL_ROW },
/* r82 */ { ROAD_MARK_ROW },
/* r83 */ { ROAD_FULL_ROW },
/* r84 */ { ROAD_FULL_ROW },  /* E-W major road belt 2 end */
/* r85 */ { DIST_SW_ROW   },
/* r86 */ { DIST_GR_ROW   },
/* r87 */ { DIST_GR_ROW   },
/* r88 */ { DIST_GR_ROW   },
/* r89 */ { DIST_SW_ROW   },
/* r90 */ { DIST_GR_ROW   },
/* r91 */ { DIST_GR_ROW   },
/* r92 */ { DIST_GR_ROW   },
/* r93 */ { DIST_SW_ROW   },
/* r94 */ { DIST_GR_ROW   },
/* r95 */ { DIST_GR_ROW   },
/* r96 */ { DIST_GR_ROW   },
/* r97 */ { DIST_SW_ROW   },
/* r98 */ { DIST_GR_ROW   },
/* r99 */ { DIST_GR_ROW   },
/* r100*/ { DIST_GR_ROW   },
/* r101*/ { DIST_SW_ROW   },
/* r102*/ { DIST_GR_ROW   },
/* r103*/ { DIST_GR_ROW   },
/* r104*/ { DIST_GR_ROW   },
/* r105*/ { DIST_SW_ROW   },
/* r106*/ { DIST_GR_ROW   },
/* r107*/ { DIST_GR_ROW   },
/* r108*/ { DIST_GR_ROW   },
/* r109*/ { DIST_SW_ROW   },
/* r110*/ { DIST_GR_ROW   },
/* r111*/ { DIST_GR_ROW   },
/* r112*/ { DIST_GR_ROW   },
/* r113*/ { DIST_SW_ROW   },
/* r114*/ { DIST_GR_ROW   },
/* r115*/ { DIST_GR_ROW   },
/* r116*/ { DIST_GR_ROW   },
/* r117*/ { DIST_SW_ROW   },
/* r118*/ { BORDER_SW_ROW },
/* r119*/ { BORDER_SW_ROW },
};

/*===========================================================================
 * LAYER 0 – GROUND FLAGS (collision)
 *
 * Rules applied:
 *  - Border rows (0-1, 118-119): fully solid (no walking off-map)
 *  - Road rows (30-34, 55-59, 80-84): COL_SOLID (vehicles only)
 *  - N-S road artery cols (4-9, 50-59, 110-115): COL_SOLID in non-intersection rows
 *  - Sidewalk rows: walkable (COL_NONE) – player path
 *  - Grass/district cells: walkable (COL_NONE) until a fringe prop overrides
 *
 * Note: fringe-layer solid props (buildings, trees, structures) are marked
 * COL_SOLID here at their footprint coordinates so the player cannot clip.
 *===========================================================================*/

/* Collision macros per column segment (used per row type) */
/* Border row – all solid */
#define CFLAG_BORDER \
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S

/* Road row – solid throughout */
#define CFLAG_ROAD \
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S

/*
 * District walkable row:
 *   cols 0-3   = border sidewalk → solid edges
 *   cols 4-9   = N-S road artery → solid
 *   cols 10-49 = walkable district
 *   cols 50-59 = central N-S road artery → solid
 *   cols 60-109= walkable district
 *   cols 110-115= E road artery → solid
 *   cols 116-119= border sidewalk → solid edges
 */
#define CFLAG_DIST \
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,\
    _O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,\
    _O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,\
    _O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_O,\
    _O,_O,_O,_O,_O,_O,_O,_O,_O,_O,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S

static const uint8_t g_groundFlags[SAMPLE_MAP_H][SAMPLE_MAP_W] = {
/* r0  */ { CFLAG_BORDER },
/* r1  */ { CFLAG_BORDER },
/* r2  */ { CFLAG_DIST   },
/* r3  */ { CFLAG_DIST   },
/* r4  */ { CFLAG_DIST   },
/* r5  */ { CFLAG_DIST   },
/* r6  */ { CFLAG_DIST   },
/* r7  */ { CFLAG_DIST   },
/* r8  */ { CFLAG_DIST   },
/* r9  */ { CFLAG_DIST   },
/* r10 */ { CFLAG_DIST   },
/* r11 */ { CFLAG_DIST   },
/* r12 */ { CFLAG_DIST   },
/* r13 */ { CFLAG_DIST   },
/* r14 */ { CFLAG_DIST   },
/* r15 */ { CFLAG_DIST   },
/* r16 */ { CFLAG_DIST   },
/* r17 */ { CFLAG_DIST   },
/* r18 */ { CFLAG_DIST   },
/* r19 */ { CFLAG_DIST   },
/* r20 */ { CFLAG_DIST   },
/* r21 */ { CFLAG_DIST   },
/* r22 */ { CFLAG_DIST   },
/* r23 */ { CFLAG_DIST   },
/* r24 */ { CFLAG_DIST   },
/* r25 */ { CFLAG_DIST   },
/* r26 */ { CFLAG_DIST   },
/* r27 */ { CFLAG_DIST   },
/* r28 */ { CFLAG_DIST   },
/* r29 */ { CFLAG_DIST   },
/* r30 */ { CFLAG_ROAD   },
/* r31 */ { CFLAG_ROAD   },
/* r32 */ { CFLAG_ROAD   },
/* r33 */ { CFLAG_ROAD   },
/* r34 */ { CFLAG_ROAD   },
/* r35 */ { CFLAG_DIST   },
/* r36 */ { CFLAG_DIST   },
/* r37 */ { CFLAG_DIST   },
/* r38 */ { CFLAG_DIST   },
/* r39 */ { CFLAG_DIST   },
/* r40 */ { CFLAG_DIST   },
/* r41 */ { CFLAG_DIST   },
/* r42 */ { CFLAG_DIST   },
/* r43 */ { CFLAG_DIST   },
/* r44 */ { CFLAG_DIST   },
/* r45 */ { CFLAG_DIST   },
/* r46 */ { CFLAG_DIST   },
/* r47 */ { CFLAG_DIST   },
/* r48 */ { CFLAG_DIST   },
/* r49 */ { CFLAG_DIST   },
/* r50 */ { CFLAG_DIST   },
/* r51 */ { CFLAG_DIST   },
/* r52 */ { CFLAG_DIST   },
/* r53 */ { CFLAG_DIST   },
/* r54 */ { CFLAG_DIST   },
/* r55 */ { CFLAG_ROAD   },
/* r56 */ { CFLAG_ROAD   },
/* r57 */ { CFLAG_ROAD   },
/* r58 */ { CFLAG_ROAD   },
/* r59 */ { CFLAG_ROAD   },
/* r60 */ { CFLAG_DIST   },
/* r61 */ { CFLAG_DIST   },
/* r62 */ { CFLAG_DIST   },
/* r63 */ { CFLAG_DIST   },
/* r64 */ { CFLAG_DIST   },
/* r65 */ { CFLAG_DIST   },
/* r66 */ { CFLAG_DIST   },
/* r67 */ { CFLAG_DIST   },
/* r68 */ { CFLAG_DIST   },
/* r69 */ { CFLAG_DIST   },
/* r70 */ { CFLAG_DIST   },
/* r71 */ { CFLAG_DIST   },
/* r72 */ { CFLAG_DIST   },
/* r73 */ { CFLAG_DIST   },
/* r74 */ { CFLAG_DIST   },
/* r75 */ { CFLAG_DIST   },
/* r76 */ { CFLAG_DIST   },
/* r77 */ { CFLAG_DIST   },
/* r78 */ { CFLAG_DIST   },
/* r79 */ { CFLAG_DIST   },
/* r80 */ { CFLAG_ROAD   },
/* r81 */ { CFLAG_ROAD   },
/* r82 */ { CFLAG_ROAD   },
/* r83 */ { CFLAG_ROAD   },
/* r84 */ { CFLAG_ROAD   },
/* r85 */ { CFLAG_DIST   },
/* r86 */ { CFLAG_DIST   },
/* r87 */ { CFLAG_DIST   },
/* r88 */ { CFLAG_DIST   },
/* r89 */ { CFLAG_DIST   },
/* r90 */ { CFLAG_DIST   },
/* r91 */ { CFLAG_DIST   },
/* r92 */ { CFLAG_DIST   },
/* r93 */ { CFLAG_DIST   },
/* r94 */ { CFLAG_DIST   },
/* r95 */ { CFLAG_DIST   },
/* r96 */ { CFLAG_DIST   },
/* r97 */ { CFLAG_DIST   },
/* r98 */ { CFLAG_DIST   },
/* r99 */ { CFLAG_DIST   },
/* r100*/ { CFLAG_DIST   },
/* r101*/ { CFLAG_DIST   },
/* r102*/ { CFLAG_DIST   },
/* r103*/ { CFLAG_DIST   },
/* r104*/ { CFLAG_DIST   },
/* r105*/ { CFLAG_DIST   },
/* r106*/ { CFLAG_DIST   },
/* r107*/ { CFLAG_DIST   },
/* r108*/ { CFLAG_DIST   },
/* r109*/ { CFLAG_DIST   },
/* r110*/ { CFLAG_DIST   },
/* r111*/ { CFLAG_DIST   },
/* r112*/ { CFLAG_DIST   },
/* r113*/ { CFLAG_DIST   },
/* r114*/ { CFLAG_DIST   },
/* r115*/ { CFLAG_DIST   },
/* r116*/ { CFLAG_DIST   },
/* r117*/ { CFLAG_DIST   },
/* r118*/ { CFLAG_BORDER },
/* r119*/ { CFLAG_BORDER },
};

/*===========================================================================
 * LAYER 1 – FRINGE DATA
 * Props, building bases, vehicles.  TILE_EMPTY (-1) = transparent.
 *
 * Districts populated:
 *   SHIBUYA   (rows 2-29)  : red/glass shop row, trees, vending machines
 *   SHINJUKU  (rows 35-54) : blue/glass tower blocks, Tokyo Tower, Sky Tree
 *   RAIL BELT (rows 55-59) : long Shinkansen train sets running E-W
 *   AKIHABARA (rows 60-79) : tech shops, vending machine clusters
 *   UENO PARK (rows 85-104): cherry blossom trees, benches, park lamps
 *   ASAKUSA   (rows 105-117): torii gate, pagoda, torii base, phone boxes
 *===========================================================================*/

/* Fully empty fringe row (120 EMPTYs) */
#define FRINGE_EMPTY \
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,\
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,\
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,\
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,\
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,\
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__

/* Shinkansen train belt – fills cols 10..113 with L/R pairs, rest empty */
#define FRINGE_SHINKANSEN_ROW \
    __,__,__,__,__,__,__,__,__,__,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    TILE_SHINKANSEN_L,TILE_SHINKANSEN_R,\
    __,__,__,__,__,__

static const int16_t g_fringeData[SAMPLE_MAP_H][SAMPLE_MAP_W] = {

/* ── r0-r1: northern border – all empty fringe ── */
/* r0  */ { FRINGE_EMPTY },
/* r1  */ { FRINGE_EMPTY },

/* ── r2: SHIBUYA – shop row (red + glass alternating) + street lamps ── */
/* r2  */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,
    __,__,__,__,__,__
},

/* r3: roof tops of row-2 buildings go in overhead layer; fringe empty row */
/* r3  */ { FRINGE_EMPTY },

/* r4: second shop row (blue blocks, police box, trash) */
/* r4  */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_BLDG_BLUE_L, TILE_BLDG_BLUE_R, __,
    TILE_BLDG_BLUE_L, TILE_BLDG_BLUE_R, __,
    TILE_POLICE_BOX,  TILE_TRASH_CAN,   __,
    TILE_BLDG_BLUE_L, TILE_BLDG_BLUE_R, __,
    TILE_BLDG_BLUE_L, TILE_BLDG_BLUE_R, __,
    TILE_POLICE_BOX,  TILE_TRASH_CAN,   __,
    TILE_BLDG_BLUE_L, TILE_BLDG_BLUE_R, __,
    TILE_BLDG_BLUE_L, TILE_BLDG_BLUE_R, __,
    TILE_POLICE_BOX,  TILE_TRASH_CAN,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_BLDG_BLUE_L, TILE_BLDG_BLUE_R, __,
    TILE_BLDG_BLUE_L, TILE_BLDG_BLUE_R, __,
    TILE_POLICE_BOX,  TILE_TRASH_CAN,   __,
    TILE_BLDG_BLUE_L, TILE_BLDG_BLUE_R, __,
    TILE_BLDG_BLUE_L, TILE_BLDG_BLUE_R, __,
    TILE_POLICE_BOX,  TILE_TRASH_CAN,   __,
    TILE_BLDG_BLUE_L, TILE_BLDG_BLUE_R, __,
    TILE_BLDG_BLUE_L, TILE_BLDG_BLUE_R, __,
    TILE_POLICE_BOX,  TILE_TRASH_CAN,
    __,__,__,__,__,__
},

/* r5  */ { FRINGE_EMPTY },

/* r6: vending machines row along sidewalk */
/* r6  */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,__,
    TILE_VENDING_A,TILE_VENDING_B,
    __,__,__,__,__,__
},

/* r7  */ { FRINGE_EMPTY },

/* r8: trees cluster west district */
/* r8  */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_CHERRY_BLOSSOM,__,__,__,
    TILE_CHERRY_BLOSSOM,__,__,__,
    TILE_CHERRY_BLOSSOM,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_CHERRY_BLOSSOM,__,__,__,
    TILE_CHERRY_BLOSSOM,__,__,__,
    TILE_CHERRY_BLOSSOM,__,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,
    __,__,__,__,__,__
},

/* r9  */ { FRINGE_EMPTY },
/* r10 */ { FRINGE_EMPTY },

/* r11: phone boxes + benches + postboxes row */
/* r11 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,
    __,__,__,__,__,__,__,__,__,__,
    __,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,
    TILE_PHONE_BOX,TILE_POSTBOX,
    __,__,__,__,__,__
},

/* r12 */ { FRINGE_EMPTY },

/* r13: more shop row (red/glass repeat) */
/* r13 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,
    __,__,__,__,__,__
},

/* r14 */ { FRINGE_EMPTY },

/* r15: trees cluster row 2 */
/* r15 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_CHERRY_BLOSSOM,__,__,__,
    TILE_CHERRY_BLOSSOM,__,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_CHERRY_BLOSSOM,__,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_CHERRY_BLOSSOM,__,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_CHERRY_BLOSSOM,__,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,
    __,__,__,__,__,__
},

/* r16 */ { FRINGE_EMPTY },

/* r17: bike racks + containers */
/* r17 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_BIKE_RACK,__,__,
    TILE_CONTAINER_L,TILE_CONTAINER_R,__,
    TILE_BIKE_RACK,__,__,
    TILE_CONTAINER_L,TILE_CONTAINER_R,__,
    TILE_BIKE_RACK,__,__,
    TILE_CONTAINER_L,TILE_CONTAINER_R,__,
    TILE_BIKE_RACK,__,__,
    TILE_CONTAINER_L,TILE_CONTAINER_R,__,
    TILE_BIKE_RACK,__,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_CONTAINER_L,TILE_CONTAINER_R,__,
    TILE_BIKE_RACK,__,__,
    TILE_CONTAINER_L,TILE_CONTAINER_R,__,
    TILE_BIKE_RACK,__,__,
    TILE_CONTAINER_L,TILE_CONTAINER_R,__,
    TILE_BIKE_RACK,__,__,
    TILE_CONTAINER_L,TILE_CONTAINER_R,__,
    TILE_BIKE_RACK,__,__,
    TILE_CONTAINER_L,TILE_CONTAINER_R,
    __,__,__,__,__,__
},

/* r18 */ { FRINGE_EMPTY },
/* r19 */ { FRINGE_EMPTY },

/* r20: awning row */
/* r20 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,__,
    TILE_AWNING_RED,__,
    __,__,__,__,__,__
},

/* r21 */ { FRINGE_EMPTY },
/* r22 */ { FRINGE_EMPTY },
/* r23 */ { FRINGE_EMPTY },

/* r24: trash cans + postboxes line */
/* r24 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_TRASH_CAN,__,__,__,
    TILE_POSTBOX,__,__,__,
    TILE_TRASH_CAN,__,__,__,
    TILE_POSTBOX,__,__,__,
    TILE_TRASH_CAN,__,__,__,
    TILE_POSTBOX,__,__,__,
    TILE_TRASH_CAN,__,__,__,
    TILE_POSTBOX,__,__,__,
    TILE_TRASH_CAN,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_POSTBOX,__,__,__,
    TILE_TRASH_CAN,__,__,__,
    TILE_POSTBOX,__,__,__,
    TILE_TRASH_CAN,__,__,__,
    TILE_POSTBOX,__,__,__,
    TILE_TRASH_CAN,__,__,__,
    TILE_POSTBOX,__,__,__,
    TILE_TRASH_CAN,__,__,__,
    TILE_POSTBOX,
    __,__,__,__,__,__
},

/* r25 */ { FRINGE_EMPTY },
/* r26 */ { FRINGE_EMPTY },
/* r27 */ { FRINGE_EMPTY },
/* r28 */ { FRINGE_EMPTY },
/* r29 */ { FRINGE_EMPTY },

/* ── r30-r34: major road belt – bus traffic ── */
/* r30 */ { FRINGE_EMPTY },
/* r31 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_BUS_L,TILE_BUS_R,__,__,
    TILE_BUS_L,TILE_BUS_R,__,__,
    TILE_BUS_L,TILE_BUS_R,__,__,
    TILE_BUS_L,TILE_BUS_R,__,__,
    TILE_BUS_L,TILE_BUS_R,__,__,
    TILE_BUS_L,TILE_BUS_R,__,__,
    TILE_BUS_L,TILE_BUS_R,__,__,
    TILE_BUS_L,TILE_BUS_R,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_BUS_L,TILE_BUS_R,__,__,
    TILE_BUS_L,TILE_BUS_R,__,__,
    TILE_BUS_L,TILE_BUS_R,__,__,
    TILE_BUS_L,TILE_BUS_R,__,__,
    TILE_BUS_L,TILE_BUS_R,__,__,
    TILE_BUS_L,TILE_BUS_R,__,__,
    TILE_BUS_L,TILE_BUS_R,__,__,
    TILE_BUS_L,TILE_BUS_R,__,__,
    TILE_BUS_L,TILE_BUS_R,
    __,__,__,__,__,__
},
/* r32 */ { FRINGE_EMPTY },
/* r33 */ { FRINGE_EMPTY },
/* r34 */ { FRINGE_EMPTY },

/* ── r35-r54: SHINJUKU WARD ── */
/* r35 */ { FRINGE_EMPTY },

/* r36: big glass skyscraper base row */
/* r36 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,
    __,__,__,__,__,__
},

/* r37 */ { FRINGE_EMPTY },

/* r38: Tokyo Tower base + Sky Tree base planted in Shinjuku plaza */
/* r38 */ {
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,
    TILE_TOKYOTOWER_BASE,   TILE_TOKYOTOWER_BASE+1,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_SKYTREE_BASE,TILE_SKYTREE_BASE+1,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__
},

/* r39 */ {
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,
    TILE_TOKYOTOWER_BASE+32, TILE_TOKYOTOWER_BASE+33,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_SKYTREE_BASE+32,TILE_SKYTREE_BASE+33,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__
},

/* r40 */ {
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,
    TILE_TOKYOTOWER_BASE+64,TILE_TOKYOTOWER_BASE+65,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_SKYTREE_BASE+64,TILE_SKYTREE_BASE+65,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__
},

/* r41 */ { FRINGE_EMPTY },
/* r42 */ { FRINGE_EMPTY },
/* r43 */ { FRINGE_EMPTY },

/* r44: blue building row (Shinjuku south) */
/* r44 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,__,
    TILE_BLDG_BLUE_L,TILE_BLDG_BLUE_R,
    __,__,__,__,__,__
},

/* r45 */ { FRINGE_EMPTY },
/* r46 */ { FRINGE_EMPTY },
/* r47 */ { FRINGE_EMPTY },
/* r48 */ { FRINGE_EMPTY },
/* r49 */ { FRINGE_EMPTY },
/* r50 */ { FRINGE_EMPTY },
/* r51 */ { FRINGE_EMPTY },
/* r52 */ { FRINGE_EMPTY },
/* r53 */ { FRINGE_EMPTY },
/* r54 */ { FRINGE_EMPTY },

/* ── r55-r59: Shinkansen rail belt ── */
/* r55 */ { FRINGE_SHINKANSEN_ROW },
/* r56 */ { FRINGE_SHINKANSEN_ROW },
/* r57 */ { FRINGE_EMPTY },
/* r58 */ { FRINGE_SHINKANSEN_ROW },
/* r59 */ { FRINGE_SHINKANSEN_ROW },

/* ── r60-r79: AKIHABARA DISTRICT ── */
/* r60 */ { FRINGE_EMPTY },

/* r61: tech shop row */
/* r61 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,__,
    TILE_BLDG_RED_L,  TILE_BLDG_RED_R,  __,
    TILE_BLDG_GLASS_L,TILE_BLDG_GLASS_R,
    __,__,__,__,__,__
},

/* r62 */ { FRINGE_EMPTY },

/* r63: vending machine wall (Akihabara style) */
/* r63 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_VENDING_A,TILE_VENDING_B,TILE_VENDING_A,TILE_VENDING_B,
    TILE_VENDING_A,TILE_VENDING_B,TILE_VENDING_A,TILE_VENDING_B,
    TILE_VENDING_A,TILE_VENDING_B,TILE_VENDING_A,TILE_VENDING_B,
    TILE_VENDING_A,TILE_VENDING_B,TILE_VENDING_A,TILE_VENDING_B,
    TILE_VENDING_A,TILE_VENDING_B,TILE_VENDING_A,TILE_VENDING_B,
    TILE_VENDING_A,TILE_VENDING_B,TILE_VENDING_A,TILE_VENDING_B,
    TILE_VENDING_A,TILE_VENDING_B,TILE_VENDING_A,TILE_VENDING_B,
    TILE_VENDING_A,TILE_VENDING_B,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_VENDING_A,TILE_VENDING_B,TILE_VENDING_A,TILE_VENDING_B,
    TILE_VENDING_A,TILE_VENDING_B,TILE_VENDING_A,TILE_VENDING_B,
    TILE_VENDING_A,TILE_VENDING_B,TILE_VENDING_A,TILE_VENDING_B,
    TILE_VENDING_A,TILE_VENDING_B,TILE_VENDING_A,TILE_VENDING_B,
    TILE_VENDING_A,TILE_VENDING_B,TILE_VENDING_A,TILE_VENDING_B,
    TILE_VENDING_A,TILE_VENDING_B,TILE_VENDING_A,TILE_VENDING_B,
    TILE_VENDING_A,TILE_VENDING_B,TILE_VENDING_A,TILE_VENDING_B,
    TILE_VENDING_A,TILE_VENDING_B,
    __,__,__,__,__,__
},

/* r64 */ { FRINGE_EMPTY },
/* r65 */ { FRINGE_EMPTY },
/* r66 */ { FRINGE_EMPTY },
/* r67 */ { FRINGE_EMPTY },
/* r68 */ { FRINGE_EMPTY },
/* r69 */ { FRINGE_EMPTY },
/* r70 */ { FRINGE_EMPTY },
/* r71 */ { FRINGE_EMPTY },
/* r72 */ { FRINGE_EMPTY },
/* r73 */ { FRINGE_EMPTY },
/* r74 */ { FRINGE_EMPTY },
/* r75 */ { FRINGE_EMPTY },
/* r76 */ { FRINGE_EMPTY },
/* r77 */ { FRINGE_EMPTY },
/* r78 */ { FRINGE_EMPTY },
/* r79 */ { FRINGE_EMPTY },

/* ── r80-r84: 2nd major road belt – all empty fringe ── */
/* r80 */ { FRINGE_EMPTY },
/* r81 */ { FRINGE_EMPTY },
/* r82 */ { FRINGE_EMPTY },
/* r83 */ { FRINGE_EMPTY },
/* r84 */ { FRINGE_EMPTY },

/* ── r85-r104: UENO PARK ── */
/* r85 */ { FRINGE_EMPTY },

/* r86: dense cherry blossom grove */
/* r86 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,
    TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,
    TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,
    TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,
    TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,
    TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,
    TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,
    TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,
    TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,
    TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,TILE_CHERRY_BLOSSOM,__,
    TILE_CHERRY_BLOSSOM,__,
    __,__,__,__,__,__
},

/* r87 */ { FRINGE_EMPTY },

/* r88: tree grove – Ueno */
/* r88 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,__,__,
    TILE_TREE_LOWER,TILE_TREE_LOWER,
    __,__,__,__,__,__
},

/* r89-r103 */ { FRINGE_EMPTY },
/* r90 */ { FRINGE_EMPTY },
/* r91 */ { FRINGE_EMPTY },
/* r92 */ { FRINGE_EMPTY },
/* r93 */ { FRINGE_EMPTY },
/* r94 */ { FRINGE_EMPTY },
/* r95 */ { FRINGE_EMPTY },
/* r96 */ { FRINGE_EMPTY },
/* r97 */ { FRINGE_EMPTY },
/* r98 */ { FRINGE_EMPTY },
/* r99 */ { FRINGE_EMPTY },
/* r100*/ { FRINGE_EMPTY },
/* r101*/ { FRINGE_EMPTY },
/* r102*/ { FRINGE_EMPTY },
/* r103*/ { FRINGE_EMPTY },
/* r104*/ { FRINGE_EMPTY },

/* ── r105-r117: ASAKUSA WARD ── */
/* r105*/ { FRINGE_EMPTY },

/* r106: torii gate base + pagoda base */
/* r106*/ {
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,
    TILE_TORII_BASE,TILE_TORII_BASE+1,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    TILE_PAGODA5_BASE,TILE_PAGODA5_BASE+1,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__
},

/* r107*/ {
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,
    TILE_TORII_BASE+32,TILE_TORII_BASE+33,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    TILE_PAGODA5_BASE+32,TILE_PAGODA5_BASE+33,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__
},

/* r108*/ { FRINGE_EMPTY },
/* r109*/ { FRINGE_EMPTY },

/* r110: phone boxes + benches Asakusa */
/* r110*/ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,__,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,__,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,__,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,__,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,__,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,__,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,__,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,__,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,__,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,__,__,__,
    TILE_PHONE_BOX,TILE_POSTBOX,TILE_BENCH,
    __,__,__,__,__,__
},

/* r111*/ { FRINGE_EMPTY },
/* r112*/ { FRINGE_EMPTY },
/* r113*/ { FRINGE_EMPTY },
/* r114*/ { FRINGE_EMPTY },
/* r115*/ { FRINGE_EMPTY },
/* r116*/ { FRINGE_EMPTY },
/* r117*/ { FRINGE_EMPTY },
/* r118*/ { FRINGE_EMPTY },
/* r119*/ { FRINGE_EMPTY },
};

/*===========================================================================
 * LAYER 2 – OVERHEAD DATA
 * Rooftops (above building bases 1 row up), treetops (above tree trunks),
 * tower top sections (rows above tower bases in fringe).
 *
 * Rule: overhead tile sits exactly one row ABOVE the matching fringe tile.
 *===========================================================================*/

static const int16_t g_overheadData[SAMPLE_MAP_H][SAMPLE_MAP_W] = {

/* r0  */ { FRINGE_EMPTY },
/* r1  */ { FRINGE_EMPTY },

/* r2: rooftops above r2 building bases (buildings placed at r2, roofs at r1 overhead) */
/* r1 overhead – but we place the roof tile in the row ABOVE the base row.
   Base=r2 → roof overhead at r1 (already handled as FRINGE_EMPTY row 1).
   Actually we output rooftops in the same row as the base in overhead layer
   since the overhead renders after the player to create depth illusion.
   Roof L/R tiles placed at same (col,row) as base in overhead layer. */
/* r2  */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,
    __,__,__,__,__,__
},

/* r3  */ { FRINGE_EMPTY },

/* r4: rooftops above r4 blue buildings */
/* r4  */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,
    __,__,__,__,__,__
},

/* r5  */ { FRINGE_EMPTY },
/* r6  */ { FRINGE_EMPTY },
/* r7  */ { FRINGE_EMPTY },

/* r8: tree canopies above r8 tree trunks */
/* r8  */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,__,__,__,
    TILE_TREE_UPPER,__,__,__,
    TILE_TREE_UPPER,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,__,__,__,
    TILE_TREE_UPPER,__,__,__,
    TILE_TREE_UPPER,__,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,
    __,__,__,__,__,__
},

/* r9  */ { FRINGE_EMPTY },
/* r10 */ { FRINGE_EMPTY },
/* r11 */ { FRINGE_EMPTY },
/* r12 */ { FRINGE_EMPTY },
/* r13 */ { FRINGE_EMPTY },
/* r14 */ { FRINGE_EMPTY },

/* r15: tree canopies above r15 tree trunks */
/* r15 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,__,__,__,
    TILE_TREE_UPPER,__,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_TREE_UPPER,__,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,__,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,__,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,
    __,__,__,__,__,__
},

/* r16 */ { FRINGE_EMPTY },
/* r17 */ { FRINGE_EMPTY },
/* r18 */ { FRINGE_EMPTY },
/* r19 */ { FRINGE_EMPTY },
/* r20 */ { FRINGE_EMPTY },
/* r21 */ { FRINGE_EMPTY },
/* r22 */ { FRINGE_EMPTY },
/* r23 */ { FRINGE_EMPTY },
/* r24 */ { FRINGE_EMPTY },
/* r25 */ { FRINGE_EMPTY },
/* r26 */ { FRINGE_EMPTY },
/* r27 */ { FRINGE_EMPTY },
/* r28 */ { FRINGE_EMPTY },
/* r29 */ { FRINGE_EMPTY },
/* r30 */ { FRINGE_EMPTY },
/* r31 */ { FRINGE_EMPTY },
/* r32 */ { FRINGE_EMPTY },
/* r33 */ { FRINGE_EMPTY },
/* r34 */ { FRINGE_EMPTY },
/* r35 */ { FRINGE_EMPTY },

/* r36: rooftops of r36 glass skyscrapers */
/* r36 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,
    __,__,__,__,__,__
},

/* r37 */ { FRINGE_EMPTY },

/* r38: Tokyo Tower tip + Sky Tree tip (overhead, row above top fringe tile) */
/* r38 */ {
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,
    TILE_TOKYOTOWER_BASE+96, TILE_TOKYOTOWER_BASE+97,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_SKYTREE_BASE+96,TILE_SKYTREE_BASE+97,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__
},

/* r39 */ { FRINGE_EMPTY },
/* r40 */ { FRINGE_EMPTY },
/* r41 */ { FRINGE_EMPTY },
/* r42 */ { FRINGE_EMPTY },
/* r43 */ { FRINGE_EMPTY },

/* r44: rooftops of r44 blue buildings (Shinjuku south) */
/* r44 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,
    __,__,__,__,__,__
},

/* r45-r60: mostly clear overhead ── */
/* r45 */ { FRINGE_EMPTY },
/* r46 */ { FRINGE_EMPTY },
/* r47 */ { FRINGE_EMPTY },
/* r48 */ { FRINGE_EMPTY },
/* r49 */ { FRINGE_EMPTY },
/* r50 */ { FRINGE_EMPTY },
/* r51 */ { FRINGE_EMPTY },
/* r52 */ { FRINGE_EMPTY },
/* r53 */ { FRINGE_EMPTY },
/* r54 */ { FRINGE_EMPTY },
/* r55 */ { FRINGE_EMPTY },
/* r56 */ { FRINGE_EMPTY },
/* r57 */ { FRINGE_EMPTY },
/* r58 */ { FRINGE_EMPTY },
/* r59 */ { FRINGE_EMPTY },
/* r60 */ { FRINGE_EMPTY },

/* r61: rooftops of r61 Akihabara shop row */
/* r61 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,__,
    TILE_BLDG_ROOF_L,TILE_BLDG_ROOF_R,
    __,__,__,__,__,__
},

/* r62-r87: clear overhead ── */
/* r62 */ { FRINGE_EMPTY },
/* r63 */ { FRINGE_EMPTY },
/* r64 */ { FRINGE_EMPTY },
/* r65 */ { FRINGE_EMPTY },
/* r66 */ { FRINGE_EMPTY },
/* r67 */ { FRINGE_EMPTY },
/* r68 */ { FRINGE_EMPTY },
/* r69 */ { FRINGE_EMPTY },
/* r70 */ { FRINGE_EMPTY },
/* r71 */ { FRINGE_EMPTY },
/* r72 */ { FRINGE_EMPTY },
/* r73 */ { FRINGE_EMPTY },
/* r74 */ { FRINGE_EMPTY },
/* r75 */ { FRINGE_EMPTY },
/* r76 */ { FRINGE_EMPTY },
/* r77 */ { FRINGE_EMPTY },
/* r78 */ { FRINGE_EMPTY },
/* r79 */ { FRINGE_EMPTY },
/* r80 */ { FRINGE_EMPTY },
/* r81 */ { FRINGE_EMPTY },
/* r82 */ { FRINGE_EMPTY },
/* r83 */ { FRINGE_EMPTY },
/* r84 */ { FRINGE_EMPTY },
/* r85 */ { FRINGE_EMPTY },
/* r86 */ { FRINGE_EMPTY },
/* r87 */ { FRINGE_EMPTY },

/* r88: tree canopies (Ueno) */
/* r88 */ {
    __,__,__,__,__,__,__,__,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,
    __,__,__,__,__,__,__,__,__,__,
    __,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,__,__,
    TILE_TREE_UPPER,TILE_TREE_UPPER,
    __,__,__,__,__,__
},

/* r89-r119: clear overhead */
/* r89 */ { FRINGE_EMPTY },
/* r90 */ { FRINGE_EMPTY },
/* r91 */ { FRINGE_EMPTY },
/* r92 */ { FRINGE_EMPTY },
/* r93 */ { FRINGE_EMPTY },
/* r94 */ { FRINGE_EMPTY },
/* r95 */ { FRINGE_EMPTY },
/* r96 */ { FRINGE_EMPTY },
/* r97 */ { FRINGE_EMPTY },
/* r98 */ { FRINGE_EMPTY },
/* r99 */ { FRINGE_EMPTY },
/* r100*/ { FRINGE_EMPTY },
/* r101*/ { FRINGE_EMPTY },
/* r102*/ { FRINGE_EMPTY },
/* r103*/ { FRINGE_EMPTY },
/* r104*/ { FRINGE_EMPTY },
/* r105*/ { FRINGE_EMPTY },
/* r106*/ { FRINGE_EMPTY },
/* r107*/ { FRINGE_EMPTY },
/* r108*/ { FRINGE_EMPTY },
/* r109*/ { FRINGE_EMPTY },
/* r110*/ { FRINGE_EMPTY },
/* r111*/ { FRINGE_EMPTY },
/* r112*/ { FRINGE_EMPTY },
/* r113*/ { FRINGE_EMPTY },
/* r114*/ { FRINGE_EMPTY },
/* r115*/ { FRINGE_EMPTY },
/* r116*/ { FRINGE_EMPTY },
/* r117*/ { FRINGE_EMPTY },
/* r118*/ { FRINGE_EMPTY },
/* r119*/ { FRINGE_EMPTY },
};

/* ── Cleanup shorthand macros ─────────────────────────────────────────────── */
#undef __
#undef GR
#undef GD
#undef SW
#undef RD
#undef RM
#undef _O
#undef _S

#undef BORDER_SW_ROW
#undef ROAD_FULL_ROW
#undef ROAD_MARK_ROW
#undef RAIL_ROAD_ROW
#undef DIST_GR_ROW
#undef DIST_SW_ROW
#undef CFLAG_BORDER
#undef CFLAG_ROAD
#undef CFLAG_DIST
#undef FRINGE_EMPTY
#undef FRINGE_SHINKANSEN_ROW

/* ── Builder function ─────────────────────────────────────────────────────── */
/**
 * Populate `out` with the hardcoded Tokyo Metropolis map data.
 * Caller must ensure `out` is zero-initialised.
 * Returns true on success, false on memory allocation failure.
 */
static inline bool SampleMap_Build(GameMap *out)
{
    out->width  = SAMPLE_MAP_W;
    out->height = SAMPLE_MAP_H;

    if (!MapAllocLayers(out)) return false;

    MapLayerLoadFromArray(out, LAYER_GROUND,
                          (const int16_t *)g_groundData,
                          (const uint8_t *)g_groundFlags);

    MapLayerLoadFromArray(out, LAYER_FRINGE,
                          (const int16_t *)g_fringeData,
                          NULL);

    MapLayerLoadFromArray(out, LAYER_OVERHEAD,
                          (const int16_t *)g_overheadData,
                          NULL);
    return true;
}

#endif /* SAMPLE_MAP_HEADER */
