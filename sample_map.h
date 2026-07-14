/*=============================================================================
 * sample_map.h  --  "JJK Tokyo Metropolis"  (120 × 120 tiles)
 *
 * Tileset : tileset_tokyo.png  (GK Japanese City Free Pack, 32×32 px tiles)
 * Sheet   : 16 columns × 24 rows  =  384 slots  (tiles 0–378 used)
 *
 * Tile ID mapping (0-indexed = file_number - 1):
 *
 *   ── TERRAIN ──────────────────────────────────────────────────────────────
 *   SW  = 184  sidewalk stone checker A  (plain grey pavement)
 *   SWB = 185  sidewalk stone checker B  (alt shade)
 *   RDA = 200  road asphalt dark A       (vehicle carriageway)
 *   RDB = 201  road asphalt dark B
 *   RDC = 202  road asphalt dark C
 *   RDD = 203  road asphalt dark D
 *   CWA = 280  crosswalk stripe A
 *   CWB = 281  crosswalk stripe B
 *   CWC = 282  crosswalk stripe C
 *   RM  = 283  road centre dash
 *   WAT = 359  water / river dark
 *
 *   ── FRINGE PROPS ─────────────────────────────────────────────────────────
 *   WDA = 82   building wall dark A
 *   WDB = 83   building wall dark B
 *   WBA = 84   building wall brown A
 *   WBB = 85   building wall brown B
 *   WAC = 86   wall accent stripe
 *   WWN = 87   wall with window
 *   LMP = 88   lamp post
 *   LMH = 89   lamp head
 *   VRM = 90   vending machine red
 *   VBL = 91   vending machine blue
 *   VWH = 92   vending machine white
 *   ATM = 93   ATM
 *   TRC = 313  trash can
 *   BRL = 312  barrel
 *   SDL = 102  shop door left
 *   SDM = 103  shop door mid
 *   SDR = 104  shop door right
 *   NRL = 105  noren left
 *   NRR = 106  noren right
 *   TRL = 309  torii left pillar
 *   TRR = 310  torii right pillar
 *   TRB = 311  torii crossbeam
 *   SKT = 354  sakura trunk
 *   SKB = 355  sakura branch
 *   BF2 = 20   building 2F facade left
 *   BF3 = 21   building 2F facade mid
 *   BF4 = 22   building 2F facade right
 *   BF5 = 36   building 1F facade left
 *   BF6 = 37   building 1F facade mid
 *   BF7 = 38   building 1F facade right
 *   TRA = 320  traditional roof A
 *   TRD = 340  traditional wall A
 *   TRE = 341  traditional wall B
 *
 *   ── OVERHEAD ─────────────────────────────────────────────────────────────
 *   RNW = 0    roof edge NW
 *   RNE = 2    roof edge NE
 *   RPN = 1    roof peak N
 *   RPM = 17   roof peak mid
 *   SKA = 356  sakura canopy A
 *   SKC = 358  sakura canopy C
 *
 * Map layout (120×120 tiles, each 32×32 px → 3840×3840 px world):
 *
 *   Rows   0–  1  : Northern perimeter (all solid sidewalk)
 *   Rows   2– 29  : SHIBUYA DISTRICT   – shops, vending, trees
 *   Rows  30– 34  : E-W Road Belt #1   (4 lanes dark asphalt + crosswalks)
 *   Rows  35– 54  : SHINJUKU DISTRICT  – multi-floor buildings, lamp rows
 *   Rows  55– 59  : River / Canal Belt  (water tiles)
 *   Rows  60– 79  : AKIHABARA DISTRICT – shop fronts, vending walls
 *   Rows  80– 84  : E-W Road Belt #2
 *   Rows  85–104  : UENO PARK          – sakura grove, wide open pavement
 *   Rows 105–117  : ASAKUSA WARD       – torii, traditional buildings
 *   Rows 118–119  : Southern perimeter (all solid sidewalk)
 *
 *   N-S Arteries:
 *     Cols   0–  3 : West border  (solid)
 *     Cols   4–  9 : West road artery (dark asphalt)
 *     Cols  10– 49 : West district
 *     Cols  50– 59 : Central road artery (dark asphalt)
 *     Cols  60–109 : East district
 *     Cols 110–115 : East road artery
 *     Cols 116–119 : East border  (solid)
 *=============================================================================*/

#ifndef SAMPLE_MAP_HEADER
#define SAMPLE_MAP_HEADER

#include "map_defs.h"

/* ── Map dimensions ─────────────────────────────────────────────────────── */
#define SAMPLE_MAP_W   120
#define SAMPLE_MAP_H   120

/* ── Tile shorthand macros (undef'd at end of file) ─────────────────────── */
#define __   TILE_EMPTY
/* terrain */
#define SW   184   /* sidewalk A                  */
#define SWB  185   /* sidewalk B                  */
#define SWC  186   /* sidewalk C                  */
#define SWD  187   /* sidewalk D                  */
#define RDA  200   /* road asphalt A              */
#define RDB  201   /* road asphalt B              */
#define RDC  202   /* road asphalt C              */
#define RDD  203   /* road asphalt D              */
#define RDS  204   /* road scuff                  */
#define RDK  209   /* road kerb                   */
#define CWA  280   /* crosswalk A                 */
#define CWB  281   /* crosswalk B                 */
#define CWC  282   /* crosswalk C                 */
#define RM   283   /* road mark dash              */
#define WAT  359   /* water                       */
#define WTB  360   /* water B                     */
#define WTC  361   /* water C                     */
/* building wall */
#define WDA  82    /* wall dark A                 */
#define WDB  83    /* wall dark B                 */
#define WBA  84    /* wall brown A                */
#define WBB  85    /* wall brown B                */
#define WAC  86    /* wall accent                 */
#define WWN  87    /* wall window                 */
/* props */
#define LMP  88    /* lamp post                   */
#define LMH  89    /* lamp head                   */
#define VRM  90    /* vending red                 */
#define VBL  91    /* vending blue                */
#define VWH  92    /* vending white               */
#define ATM  93    /* ATM                         */
#define TRC  313   /* trash can                   */
#define BRL  312   /* barrel                      */
/* shop */
#define SDL  102   /* shop door L                 */
#define SDM  103   /* shop door M                 */
#define SDR  104   /* shop door R                 */
#define NRL  105   /* noren L                     */
#define NRR  106   /* noren R                     */
/* shrine */
#define TRL  309   /* torii L                     */
#define TRR  310   /* torii R                     */
#define TRB  311   /* torii beam                  */
/* building multi-tile */
#define BF2  20    /* 2F facade L                 */
#define BF3  21    /* 2F facade M                 */
#define BF4  22    /* 2F facade R                 */
#define BF5  36    /* 1F facade L                 */
#define BF6  37    /* 1F facade M                 */
#define BF7  38    /* 1F facade R                 */
/* sakura */
#define SKT  354   /* sakura trunk                */
#define SKB  355   /* sakura branch               */
#define SKA  356   /* sakura canopy A (overhead)  */
#define SKC  358   /* sakura canopy C (overhead)  */
/* traditional */
#define TRA  320   /* trad roof A                 */
#define TRD  340   /* trad wall A                 */
#define TRE  341   /* trad wall B                 */
/* roof (overhead) */
#define RNW  0     /* roof NW corner              */
#define RPN  1     /* roof N edge                 */
#define RNE  2     /* roof NE corner              */
#define RPM  17    /* roof peak mid               */

/* collision shorthands */
#define _O   COL_NONE
#define _S   COL_SOLID

/*===========================================================================
 * ROW MACROS  (each expands to exactly 120 values)
 *===========================================================================*/

/* All-sidewalk border row */
#define BORDER_ROW \
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,\
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,\
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,\
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,\
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,\
    SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW

/* District sidewalk row:
   0-3=border, 4-9=W road, 10-49=district(SW), 50-59=C road, 60-109=district(SW), 110-115=E road, 116-119=border */
#define DIST_SW_ROW \
    SW, SW, SW, SW,\
    RDA,RDB,RDC,RDA,RDB,RDC,\
    SW, SW, SW, SW, SW, SW, SW, SW, SW, SW,\
    SW, SW, SW, SW, SW, SW, SW, SW, SW, SW,\
    SW, SW, SW, SW, SW, SW, SW, SW, SW, SW,\
    SW, SW, SW, SW, SW, SW, SW, SW, SW, SW,\
    RDA,RDB,RDC,RDA,RDB,RDC,RDA,RDB,RDC,RDA,\
    SW, SW, SW, SW, SW, SW, SW, SW, SW, SW,\
    SW, SW, SW, SW, SW, SW, SW, SW, SW, SW,\
    SW, SW, SW, SW, SW, SW, SW, SW, SW, SW,\
    SW, SW, SW, SW, SW, SW, SW, SW, SW, SW,\
    RDA,RDB,RDC,RDA,RDB,RDC,\
    SW, SW, SW, SW

/* District grass/open row (same arteries, open pavement in districts) */
#define DIST_OP_ROW \
    SW, SW, SW, SW,\
    RDA,RDB,RDC,RDA,RDB,RDC,\
    SWB,SW, SWC,SWB,SW, SWC,SWB,SW, SWC,SWB,\
    SW, SWC,SWB,SW, SWC,SWB,SW, SWC,SWB,SW,\
    SWC,SWB,SW, SWC,SWB,SW, SWC,SWB,SW, SWC,\
    SWB,SW, SWC,SWB,SW, SWC,SWB,SW, SWC,SWB,\
    RDA,RDB,RDC,RDA,RDB,RDC,RDA,RDB,RDC,RDA,\
    SW, SWC,SWB,SW, SWC,SWB,SW, SWC,SWB,SW,\
    SWC,SWB,SW, SWC,SWB,SW, SWC,SWB,SW, SWC,\
    SWB,SW, SWC,SWB,SW, SWC,SWB,SW, SWC,SWB,\
    SW, SWC,SWB,SW, SWC,SWB,SW, SWC,SWB,SW,\
    RDA,RDB,RDC,RDA,RDB,RDC,\
    SW, SW, SW, SW

/* E-W full asphalt road row */
#define ROAD_FULL_ROW \
    SW, SW, SW, SW,\
    RDA,RDB,RDC,RDA,RDB,RDC,\
    RDA,RDB,RDC,RDA,RDB,RDC,RDA,RDB,RDC,RDA,\
    RDB,RDC,RDA,RDB,RDC,RDA,RDB,RDC,RDA,RDB,\
    RDC,RDA,RDB,RDC,RDA,RDB,RDC,RDA,RDB,RDC,\
    RDA,RDB,RDC,RDA,RDB,RDC,RDA,RDB,RDC,RDA,\
    RDA,RDB,RDC,RDA,RDB,RDC,RDA,RDB,RDC,RDA,\
    RDB,RDC,RDA,RDB,RDC,RDA,RDB,RDC,RDA,RDB,\
    RDC,RDA,RDB,RDC,RDA,RDB,RDC,RDA,RDB,RDC,\
    RDA,RDB,RDC,RDA,RDB,RDC,RDA,RDB,RDC,RDA,\
    RDB,RDC,RDA,RDB,RDC,RDA,RDB,RDC,RDA,RDB,\
    RDA,RDB,RDC,RDA,RDB,RDC,\
    SW, SW, SW, SW

/* Crosswalk / road marking row */
#define ROAD_MARK_ROW \
    SW, SW, SW, SW,\
    RDA,RDB,RDC,RDA,RDB,RDC,\
    CWA,CWB,CWC,CWA,CWB,CWC,CWA,CWB,CWC,CWA,\
    RDB,RM, RDA,RDB,RM, RDA,RDB,RM, RDA,RDB,\
    RM, RDA,RDB,RM, RDA,RDB,RM, RDA,RDB,RM,\
    CWA,CWB,CWC,CWA,CWB,CWC,CWA,CWB,CWC,CWA,\
    RDA,RM, RDB,RDA,RM, RDB,RDA,RM, RDB,RDA,\
    CWA,CWB,CWC,CWA,CWB,CWC,CWA,CWB,CWC,CWA,\
    RDB,RM, RDA,RDB,RM, RDA,RDB,RM, RDA,RDB,\
    RM, RDA,RDB,RM, RDA,RDB,RM, RDA,RDB,RM,\
    CWA,CWB,CWC,CWA,CWB,CWC,CWA,CWB,CWC,CWA,\
    RDA,RDB,RDC,RDA,RDB,RDC,\
    SW, SW, SW, SW

/* Water/river belt row */
#define RIVER_ROW \
    SW, SW, SW, SW,\
    RDA,RDB,RDC,RDA,RDB,RDC,\
    WAT,WTB,WTC,WAT,WTB,WTC,WAT,WTB,WTC,WAT,\
    WTB,WTC,WAT,WTB,WTC,WAT,WTB,WTC,WAT,WTB,\
    WTC,WAT,WTB,WTC,WAT,WTB,WTC,WAT,WTB,WTC,\
    WAT,WTB,WTC,WAT,WTB,WTC,WAT,WTB,WTC,WAT,\
    RDA,RDB,RDC,RDA,RDB,RDC,RDA,RDB,RDC,RDA,\
    WAT,WTB,WTC,WAT,WTB,WTC,WAT,WTB,WTC,WAT,\
    WTB,WTC,WAT,WTB,WTC,WAT,WTB,WTC,WAT,WTB,\
    WTC,WAT,WTB,WTC,WAT,WTB,WTC,WAT,WTB,WTC,\
    WAT,WTB,WTC,WAT,WTB,WTC,WAT,WTB,WTC,WAT,\
    RDA,RDB,RDC,RDA,RDB,RDC,\
    SW, SW, SW, SW

/*===========================================================================
 * COLLISION ROW MACROS (exactly 120 uint8_t values)
 *===========================================================================*/

#define CFLAG_BORDER \
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S

#define CFLAG_ROAD \
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S,_S

/* District walkable row:
   border(4)=solid, W-road(6)=solid, district(40)=open, C-road(10)=solid,
   district(50)=open, E-road(6)=solid, border(4)=solid */
#define CFLAG_DIST \
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _O,_O,_O,_O,_O,_O,_O,_O,_O,_O,\
    _O,_O,_O,_O,_O,_O,_O,_O,_O,_O,\
    _O,_O,_O,_O,_O,_O,_O,_O,_O,_O,\
    _O,_O,_O,_O,_O,_O,_O,_O,_O,_O,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _O,_O,_O,_O,_O,_O,_O,_O,_O,_O,\
    _O,_O,_O,_O,_O,_O,_O,_O,_O,_O,\
    _O,_O,_O,_O,_O,_O,_O,_O,_O,_O,\
    _O,_O,_O,_O,_O,_O,_O,_O,_O,_O,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S,\
    _S,_S,_S,_S,_S,_S,_S,_S,_S,_S

/*===========================================================================
 * GROUND DATA  (terrain tile IDs, 120×120)
 *===========================================================================*/
static const int16_t g_groundData[SAMPLE_MAP_H][SAMPLE_MAP_W] = {
/* r0   */ { BORDER_ROW },
/* r1   */ { BORDER_ROW },
/* r2   */ { DIST_SW_ROW },
/* r3   */ { DIST_OP_ROW },
/* r4   */ { DIST_OP_ROW },
/* r5   */ { DIST_OP_ROW },
/* r6   */ { DIST_SW_ROW },
/* r7   */ { DIST_OP_ROW },
/* r8   */ { DIST_OP_ROW },
/* r9   */ { DIST_OP_ROW },
/* r10  */ { DIST_SW_ROW },
/* r11  */ { DIST_OP_ROW },
/* r12  */ { DIST_OP_ROW },
/* r13  */ { DIST_OP_ROW },
/* r14  */ { DIST_SW_ROW },
/* r15  */ { DIST_OP_ROW },
/* r16  */ { DIST_OP_ROW },
/* r17  */ { DIST_OP_ROW },
/* r18  */ { DIST_SW_ROW },
/* r19  */ { DIST_OP_ROW },
/* r20  */ { DIST_OP_ROW },
/* r21  */ { DIST_OP_ROW },
/* r22  */ { DIST_SW_ROW },
/* r23  */ { DIST_OP_ROW },
/* r24  */ { DIST_OP_ROW },
/* r25  */ { DIST_OP_ROW },
/* r26  */ { DIST_SW_ROW },
/* r27  */ { DIST_OP_ROW },
/* r28  */ { DIST_OP_ROW },
/* r29  */ { DIST_OP_ROW },
/* r30  */ { ROAD_FULL_ROW },
/* r31  */ { ROAD_FULL_ROW },
/* r32  */ { ROAD_MARK_ROW },
/* r33  */ { ROAD_FULL_ROW },
/* r34  */ { ROAD_FULL_ROW },
/* r35  */ { DIST_SW_ROW },
/* r36  */ { DIST_OP_ROW },
/* r37  */ { DIST_OP_ROW },
/* r38  */ { DIST_OP_ROW },
/* r39  */ { DIST_SW_ROW },
/* r40  */ { DIST_OP_ROW },
/* r41  */ { DIST_OP_ROW },
/* r42  */ { DIST_OP_ROW },
/* r43  */ { DIST_SW_ROW },
/* r44  */ { DIST_OP_ROW },
/* r45  */ { DIST_OP_ROW },
/* r46  */ { DIST_OP_ROW },
/* r47  */ { DIST_SW_ROW },
/* r48  */ { DIST_OP_ROW },
/* r49  */ { DIST_OP_ROW },
/* r50  */ { DIST_OP_ROW },
/* r51  */ { DIST_SW_ROW },
/* r52  */ { DIST_OP_ROW },
/* r53  */ { DIST_OP_ROW },
/* r54  */ { DIST_OP_ROW },
/* r55  */ { RIVER_ROW },
/* r56  */ { RIVER_ROW },
/* r57  */ { RIVER_ROW },
/* r58  */ { RIVER_ROW },
/* r59  */ { RIVER_ROW },
/* r60  */ { DIST_SW_ROW },
/* r61  */ { DIST_OP_ROW },
/* r62  */ { DIST_OP_ROW },
/* r63  */ { DIST_OP_ROW },
/* r64  */ { DIST_SW_ROW },
/* r65  */ { DIST_OP_ROW },
/* r66  */ { DIST_OP_ROW },
/* r67  */ { DIST_OP_ROW },
/* r68  */ { DIST_SW_ROW },
/* r69  */ { DIST_OP_ROW },
/* r70  */ { DIST_OP_ROW },
/* r71  */ { DIST_OP_ROW },
/* r72  */ { DIST_SW_ROW },
/* r73  */ { DIST_OP_ROW },
/* r74  */ { DIST_OP_ROW },
/* r75  */ { DIST_OP_ROW },
/* r76  */ { DIST_SW_ROW },
/* r77  */ { DIST_OP_ROW },
/* r78  */ { DIST_OP_ROW },
/* r79  */ { DIST_OP_ROW },
/* r80  */ { ROAD_FULL_ROW },
/* r81  */ { ROAD_FULL_ROW },
/* r82  */ { ROAD_MARK_ROW },
/* r83  */ { ROAD_FULL_ROW },
/* r84  */ { ROAD_FULL_ROW },
/* r85  */ { DIST_SW_ROW },
/* r86  */ { DIST_OP_ROW },
/* r87  */ { DIST_OP_ROW },
/* r88  */ { DIST_OP_ROW },
/* r89  */ { DIST_SW_ROW },
/* r90  */ { DIST_OP_ROW },
/* r91  */ { DIST_OP_ROW },
/* r92  */ { DIST_OP_ROW },
/* r93  */ { DIST_SW_ROW },
/* r94  */ { DIST_OP_ROW },
/* r95  */ { DIST_OP_ROW },
/* r96  */ { DIST_OP_ROW },
/* r97  */ { DIST_SW_ROW },
/* r98  */ { DIST_OP_ROW },
/* r99  */ { DIST_OP_ROW },
/* r100 */ { DIST_OP_ROW },
/* r101 */ { DIST_SW_ROW },
/* r102 */ { DIST_OP_ROW },
/* r103 */ { DIST_OP_ROW },
/* r104 */ { DIST_OP_ROW },
/* r105 */ { DIST_SW_ROW },
/* r106 */ { DIST_OP_ROW },
/* r107 */ { DIST_OP_ROW },
/* r108 */ { DIST_OP_ROW },
/* r109 */ { DIST_SW_ROW },
/* r110 */ { DIST_OP_ROW },
/* r111 */ { DIST_OP_ROW },
/* r112 */ { DIST_OP_ROW },
/* r113 */ { DIST_SW_ROW },
/* r114 */ { DIST_OP_ROW },
/* r115 */ { DIST_OP_ROW },
/* r116 */ { DIST_OP_ROW },
/* r117 */ { DIST_SW_ROW },
/* r118 */ { BORDER_ROW },
/* r119 */ { BORDER_ROW },
};

/*===========================================================================
 * GROUND FLAGS  (collision, 120×120)
 *===========================================================================*/
static const uint8_t g_groundFlags[SAMPLE_MAP_H][SAMPLE_MAP_W] = {
/* r0   */ { CFLAG_BORDER },
/* r1   */ { CFLAG_BORDER },
/* r2   */ { CFLAG_DIST },
/* r3   */ { CFLAG_DIST },
/* r4   */ { CFLAG_DIST },
/* r5   */ { CFLAG_DIST },
/* r6   */ { CFLAG_DIST },
/* r7   */ { CFLAG_DIST },
/* r8   */ { CFLAG_DIST },
/* r9   */ { CFLAG_DIST },
/* r10  */ { CFLAG_DIST },
/* r11  */ { CFLAG_DIST },
/* r12  */ { CFLAG_DIST },
/* r13  */ { CFLAG_DIST },
/* r14  */ { CFLAG_DIST },
/* r15  */ { CFLAG_DIST },
/* r16  */ { CFLAG_DIST },
/* r17  */ { CFLAG_DIST },
/* r18  */ { CFLAG_DIST },
/* r19  */ { CFLAG_DIST },
/* r20  */ { CFLAG_DIST },
/* r21  */ { CFLAG_DIST },
/* r22  */ { CFLAG_DIST },
/* r23  */ { CFLAG_DIST },
/* r24  */ { CFLAG_DIST },
/* r25  */ { CFLAG_DIST },
/* r26  */ { CFLAG_DIST },
/* r27  */ { CFLAG_DIST },
/* r28  */ { CFLAG_DIST },
/* r29  */ { CFLAG_DIST },
/* r30  */ { CFLAG_ROAD },
/* r31  */ { CFLAG_ROAD },
/* r32  */ { CFLAG_ROAD },
/* r33  */ { CFLAG_ROAD },
/* r34  */ { CFLAG_ROAD },
/* r35  */ { CFLAG_DIST },
/* r36  */ { CFLAG_DIST },
/* r37  */ { CFLAG_DIST },
/* r38  */ { CFLAG_DIST },
/* r39  */ { CFLAG_DIST },
/* r40  */ { CFLAG_DIST },
/* r41  */ { CFLAG_DIST },
/* r42  */ { CFLAG_DIST },
/* r43  */ { CFLAG_DIST },
/* r44  */ { CFLAG_DIST },
/* r45  */ { CFLAG_DIST },
/* r46  */ { CFLAG_DIST },
/* r47  */ { CFLAG_DIST },
/* r48  */ { CFLAG_DIST },
/* r49  */ { CFLAG_DIST },
/* r50  */ { CFLAG_DIST },
/* r51  */ { CFLAG_DIST },
/* r52  */ { CFLAG_DIST },
/* r53  */ { CFLAG_DIST },
/* r54  */ { CFLAG_DIST },
/* r55  */ { CFLAG_ROAD },  /* river = impassable */
/* r56  */ { CFLAG_ROAD },
/* r57  */ { CFLAG_ROAD },
/* r58  */ { CFLAG_ROAD },
/* r59  */ { CFLAG_ROAD },
/* r60  */ { CFLAG_DIST },
/* r61  */ { CFLAG_DIST },
/* r62  */ { CFLAG_DIST },
/* r63  */ { CFLAG_DIST },
/* r64  */ { CFLAG_DIST },
/* r65  */ { CFLAG_DIST },
/* r66  */ { CFLAG_DIST },
/* r67  */ { CFLAG_DIST },
/* r68  */ { CFLAG_DIST },
/* r69  */ { CFLAG_DIST },
/* r70  */ { CFLAG_DIST },
/* r71  */ { CFLAG_DIST },
/* r72  */ { CFLAG_DIST },
/* r73  */ { CFLAG_DIST },
/* r74  */ { CFLAG_DIST },
/* r75  */ { CFLAG_DIST },
/* r76  */ { CFLAG_DIST },
/* r77  */ { CFLAG_DIST },
/* r78  */ { CFLAG_DIST },
/* r79  */ { CFLAG_DIST },
/* r80  */ { CFLAG_ROAD },
/* r81  */ { CFLAG_ROAD },
/* r82  */ { CFLAG_ROAD },
/* r83  */ { CFLAG_ROAD },
/* r84  */ { CFLAG_ROAD },
/* r85  */ { CFLAG_DIST },
/* r86  */ { CFLAG_DIST },
/* r87  */ { CFLAG_DIST },
/* r88  */ { CFLAG_DIST },
/* r89  */ { CFLAG_DIST },
/* r90  */ { CFLAG_DIST },
/* r91  */ { CFLAG_DIST },
/* r92  */ { CFLAG_DIST },
/* r93  */ { CFLAG_DIST },
/* r94  */ { CFLAG_DIST },
/* r95  */ { CFLAG_DIST },
/* r96  */ { CFLAG_DIST },
/* r97  */ { CFLAG_DIST },
/* r98  */ { CFLAG_DIST },
/* r99  */ { CFLAG_DIST },
/* r100 */ { CFLAG_DIST },
/* r101 */ { CFLAG_DIST },
/* r102 */ { CFLAG_DIST },
/* r103 */ { CFLAG_DIST },
/* r104 */ { CFLAG_DIST },
/* r105 */ { CFLAG_DIST },
/* r106 */ { CFLAG_DIST },
/* r107 */ { CFLAG_DIST },
/* r108 */ { CFLAG_DIST },
/* r109 */ { CFLAG_DIST },
/* r110 */ { CFLAG_DIST },
/* r111 */ { CFLAG_DIST },
/* r112 */ { CFLAG_DIST },
/* r113 */ { CFLAG_DIST },
/* r114 */ { CFLAG_DIST },
/* r115 */ { CFLAG_DIST },
/* r116 */ { CFLAG_DIST },
/* r117 */ { CFLAG_DIST },
/* r118 */ { CFLAG_BORDER },
/* r119 */ { CFLAG_BORDER },
};

/*===========================================================================
 * FRINGE DATA  (building bases, props – renders above player bottom half)
 *
 * EMPTY ROW macro for convenience
 *===========================================================================*/

#define FE \
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,\
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,\
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,\
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,\
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,\
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__

static const int16_t g_fringeData[SAMPLE_MAP_H][SAMPLE_MAP_W] = {

/* ── r0-r1: northern border ── */
/* r0  */ { FE },
/* r1  */ { FE },

/* ── SHIBUYA DISTRICT rows 2-29 ── */

/* r2: shop row – sliding doors and noren curtains */
/* r2  */ {
    __,__,__,__,__,__,__,__,__,__,
    SDL,SDM,SDR,__,SDL,SDM,SDR,__,SDL,SDM,
    SDR,__,SDL,SDM,SDR,__,SDL,SDM,SDR,__,
    SDL,SDM,SDR,__,SDL,SDM,SDR,__,SDL,SDM,
    SDR,__,SDL,SDM,SDR,__,SDL,SDM,SDR,__,
    __,__,__,__,__,__,__,__,__,__,
    SDL,SDM,SDR,__,SDL,SDM,SDR,__,SDL,SDM,
    SDR,__,SDL,SDM,SDR,__,SDL,SDM,SDR,__,
    SDL,SDM,SDR,__,SDL,SDM,SDR,__,SDL,SDM,
    SDR,__,SDL,SDM,SDR,__,SDL,SDM,SDR,__,
    SDL,SDM,SDR,__,SDL,SDM,SDR,__,SDL,SDM,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r3  */ {
    __,__,__,__,__,__,__,__,__,__,
    NRL,NRR,__,__,NRL,NRR,__,__,NRL,NRR,
    __,__,NRL,NRR,__,__,NRL,NRR,__,__,
    NRL,NRR,__,__,NRL,NRR,__,__,NRL,NRR,
    __,__,NRL,NRR,__,__,NRL,NRR,__,__,
    __,__,__,__,__,__,__,__,__,__,
    NRL,NRR,__,__,NRL,NRR,__,__,NRL,NRR,
    __,__,NRL,NRR,__,__,NRL,NRR,__,__,
    NRL,NRR,__,__,NRL,NRR,__,__,NRL,NRR,
    __,__,NRL,NRR,__,__,NRL,NRR,__,__,
    NRL,NRR,__,__,NRL,NRR,__,__,NRL,NRR,
    __,__,__,__,__,__,
    __,__,__,__
},

/* r4: vending machine rows */
/* r4  */ {
    __,__,__,__,__,__,__,__,__,__,
    VRM,VBL,VWH,__,VRM,VBL,VWH,__,VRM,VBL,
    VWH,__,VRM,VBL,VWH,__,VRM,VBL,VWH,__,
    VRM,VBL,VWH,__,VRM,VBL,VWH,__,VRM,VBL,
    VWH,__,VRM,VBL,VWH,__,VRM,VBL,VWH,__,
    __,__,__,__,__,__,__,__,__,__,
    VRM,VBL,VWH,__,VRM,VBL,VWH,__,VRM,VBL,
    VWH,__,VRM,VBL,VWH,__,VRM,VBL,VWH,__,
    VRM,VBL,VWH,__,VRM,VBL,VWH,__,VRM,VBL,
    VWH,__,VRM,VBL,VWH,__,VRM,VBL,VWH,__,
    VRM,VBL,VWH,__,VRM,VBL,VWH,__,VRM,VBL,
    __,__,__,__,__,__,
    __,__,__,__
},

/* r5  */ { FE },

/* r6: building wall facades */
/* r6  */ {
    __,__,__,__,__,__,__,__,__,__,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    __,__,__,__,__,__,__,__,__,__,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    __,__,__,__,__,__,
    __,__,__,__
},

/* r7: street lamps */
/* r7  */ {
    __,__,__,__,__,__,__,__,__,__,
    LMH,__,__,__,LMH,__,__,__,LMH,__,
    __,__,LMH,__,__,__,LMH,__,__,__,
    LMH,__,__,__,LMH,__,__,__,LMH,__,
    __,__,LMH,__,__,__,LMH,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    LMH,__,__,__,LMH,__,__,__,LMH,__,
    __,__,LMH,__,__,__,LMH,__,__,__,
    LMH,__,__,__,LMH,__,__,__,LMH,__,
    __,__,LMH,__,__,__,LMH,__,__,__,
    LMH,__,__,__,LMH,__,__,__,LMH,__,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r8  */ {
    __,__,__,__,__,__,__,__,__,__,
    LMP,__,__,__,LMP,__,__,__,LMP,__,
    __,__,LMP,__,__,__,LMP,__,__,__,
    LMP,__,__,__,LMP,__,__,__,LMP,__,
    __,__,LMP,__,__,__,LMP,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    LMP,__,__,__,LMP,__,__,__,LMP,__,
    __,__,LMP,__,__,__,LMP,__,__,__,
    LMP,__,__,__,LMP,__,__,__,LMP,__,
    __,__,LMP,__,__,__,LMP,__,__,__,
    LMP,__,__,__,LMP,__,__,__,LMP,__,
    __,__,__,__,__,__,
    __,__,__,__
},

/* r9: ATM + wall accents */
/* r9  */ {
    __,__,__,__,__,__,__,__,__,__,
    ATM,__,WAC,__,ATM,__,WAC,__,ATM,__,
    WAC,__,ATM,__,WAC,__,ATM,__,WAC,__,
    ATM,__,WAC,__,ATM,__,WAC,__,ATM,__,
    WAC,__,ATM,__,WAC,__,ATM,__,WAC,__,
    __,__,__,__,__,__,__,__,__,__,
    ATM,__,WAC,__,ATM,__,WAC,__,ATM,__,
    WAC,__,ATM,__,WAC,__,ATM,__,WAC,__,
    ATM,__,WAC,__,ATM,__,WAC,__,ATM,__,
    WAC,__,ATM,__,WAC,__,ATM,__,WAC,__,
    ATM,__,WAC,__,ATM,__,WAC,__,ATM,__,
    __,__,__,__,__,__,
    __,__,__,__
},

/* r10: building 2F facades */
/* r10 */ {
    __,__,__,__,__,__,__,__,__,__,
    BF2,BF3,BF4,__,BF2,BF3,BF4,__,BF2,BF3,
    BF4,__,BF2,BF3,BF4,__,BF2,BF3,BF4,__,
    BF2,BF3,BF4,__,BF2,BF3,BF4,__,BF2,BF3,
    BF4,__,BF2,BF3,BF4,__,BF2,BF3,BF4,__,
    __,__,__,__,__,__,__,__,__,__,
    BF2,BF3,BF4,__,BF2,BF3,BF4,__,BF2,BF3,
    BF4,__,BF2,BF3,BF4,__,BF2,BF3,BF4,__,
    BF2,BF3,BF4,__,BF2,BF3,BF4,__,BF2,BF3,
    BF4,__,BF2,BF3,BF4,__,BF2,BF3,BF4,__,
    BF2,BF3,BF4,__,BF2,BF3,BF4,__,BF2,BF3,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r11 */ {
    __,__,__,__,__,__,__,__,__,__,
    BF5,BF6,BF7,__,BF5,BF6,BF7,__,BF5,BF6,
    BF7,__,BF5,BF6,BF7,__,BF5,BF6,BF7,__,
    BF5,BF6,BF7,__,BF5,BF6,BF7,__,BF5,BF6,
    BF7,__,BF5,BF6,BF7,__,BF5,BF6,BF7,__,
    __,__,__,__,__,__,__,__,__,__,
    BF5,BF6,BF7,__,BF5,BF6,BF7,__,BF5,BF6,
    BF7,__,BF5,BF6,BF7,__,BF5,BF6,BF7,__,
    BF5,BF6,BF7,__,BF5,BF6,BF7,__,BF5,BF6,
    BF7,__,BF5,BF6,BF7,__,BF5,BF6,BF7,__,
    BF5,BF6,BF7,__,BF5,BF6,BF7,__,BF5,BF6,
    __,__,__,__,__,__,
    __,__,__,__
},

/* r12-r13: brown wall accents */
/* r12 */ {
    __,__,__,__,__,__,__,__,__,__,
    WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,
    WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,
    WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,
    WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,
    __,__,__,__,__,__,__,__,__,__,
    WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,
    WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,
    WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,
    WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,
    WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,WBA,WBB,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r13 */ { FE },

/* r14: sakura trees planted */
/* r14 */ {
    __,__,__,__,__,__,__,__,__,__,
    SKT,__,__,SKT,__,__,SKT,__,__,SKT,
    __,__,SKT,__,__,SKT,__,__,SKT,__,
    __,SKT,__,__,SKT,__,__,SKT,__,__,
    SKT,__,__,SKT,__,__,SKT,__,__,SKT,
    __,__,__,__,__,__,__,__,__,__,
    SKT,__,__,SKT,__,__,SKT,__,__,SKT,
    __,__,SKT,__,__,SKT,__,__,SKT,__,
    __,SKT,__,__,SKT,__,__,SKT,__,__,
    SKT,__,__,SKT,__,__,SKT,__,__,SKT,
    __,__,SKT,__,__,SKT,__,__,SKT,__,
    __,__,__,__,__,__,
    __,__,__,__
},

/* r15: sakura branch layer */
/* r15 */ {
    __,__,__,__,__,__,__,__,__,__,
    SKB,__,__,SKB,__,__,SKB,__,__,SKB,
    __,__,SKB,__,__,SKB,__,__,SKB,__,
    __,SKB,__,__,SKB,__,__,SKB,__,__,
    SKB,__,__,SKB,__,__,SKB,__,__,SKB,
    __,__,__,__,__,__,__,__,__,__,
    SKB,__,__,SKB,__,__,SKB,__,__,SKB,
    __,__,SKB,__,__,SKB,__,__,SKB,__,
    __,SKB,__,__,SKB,__,__,SKB,__,__,
    SKB,__,__,SKB,__,__,SKB,__,__,SKB,
    __,__,SKB,__,__,SKB,__,__,SKB,__,
    __,__,__,__,__,__,
    __,__,__,__
},

/* r16-r17: trash cans + barrel clusters */
/* r16 */ {
    __,__,__,__,__,__,__,__,__,__,
    TRC,__,BRL,__,TRC,__,BRL,__,TRC,__,
    BRL,__,TRC,__,BRL,__,TRC,__,BRL,__,
    TRC,__,BRL,__,TRC,__,BRL,__,TRC,__,
    BRL,__,TRC,__,BRL,__,TRC,__,BRL,__,
    __,__,__,__,__,__,__,__,__,__,
    TRC,__,BRL,__,TRC,__,BRL,__,TRC,__,
    BRL,__,TRC,__,BRL,__,TRC,__,BRL,__,
    TRC,__,BRL,__,TRC,__,BRL,__,TRC,__,
    BRL,__,TRC,__,BRL,__,TRC,__,BRL,__,
    TRC,__,BRL,__,TRC,__,BRL,__,TRC,__,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r17 */ { FE },
/* r18 */ { FE },
/* r19 */ { FE },
/* r20 */ { FE },
/* r21 */ { FE },
/* r22 */ { FE },
/* r23 */ { FE },
/* r24 */ { FE },
/* r25 */ { FE },
/* r26 */ { FE },
/* r27 */ { FE },
/* r28 */ { FE },
/* r29 */ { FE },

/* ── E-W ROAD BELT rows 30-34: fringe clear ── */
/* r30 */ { FE },
/* r31 */ { FE },
/* r32 */ { FE },
/* r33 */ { FE },
/* r34 */ { FE },

/* ── SHINJUKU rows 35-54 ── */
/* r35 */ { FE },
/* r36 */ {
    __,__,__,__,__,__,__,__,__,__,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    __,__,__,__,__,__,__,__,__,__,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r37 */ {
    __,__,__,__,__,__,__,__,__,__,
    BF2,BF3,BF4,__,BF2,BF3,BF4,__,BF2,BF3,
    BF4,__,BF2,BF3,BF4,__,BF2,BF3,BF4,__,
    BF2,BF3,BF4,__,BF2,BF3,BF4,__,BF2,BF3,
    BF4,__,BF2,BF3,BF4,__,BF2,BF3,BF4,__,
    __,__,__,__,__,__,__,__,__,__,
    BF2,BF3,BF4,__,BF2,BF3,BF4,__,BF2,BF3,
    BF4,__,BF2,BF3,BF4,__,BF2,BF3,BF4,__,
    BF2,BF3,BF4,__,BF2,BF3,BF4,__,BF2,BF3,
    BF4,__,BF2,BF3,BF4,__,BF2,BF3,BF4,__,
    BF2,BF3,BF4,__,BF2,BF3,BF4,__,BF2,BF3,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r38 */ {
    __,__,__,__,__,__,__,__,__,__,
    BF5,BF6,BF7,__,BF5,BF6,BF7,__,BF5,BF6,
    BF7,__,BF5,BF6,BF7,__,BF5,BF6,BF7,__,
    BF5,BF6,BF7,__,BF5,BF6,BF7,__,BF5,BF6,
    BF7,__,BF5,BF6,BF7,__,BF5,BF6,BF7,__,
    __,__,__,__,__,__,__,__,__,__,
    BF5,BF6,BF7,__,BF5,BF6,BF7,__,BF5,BF6,
    BF7,__,BF5,BF6,BF7,__,BF5,BF6,BF7,__,
    BF5,BF6,BF7,__,BF5,BF6,BF7,__,BF5,BF6,
    BF7,__,BF5,BF6,BF7,__,BF5,BF6,BF7,__,
    BF5,BF6,BF7,__,BF5,BF6,BF7,__,BF5,BF6,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r39 */ {
    __,__,__,__,__,__,__,__,__,__,
    SDL,SDM,SDR,__,SDL,SDM,SDR,__,SDL,SDM,
    SDR,__,SDL,SDM,SDR,__,SDL,SDM,SDR,__,
    SDL,SDM,SDR,__,SDL,SDM,SDR,__,SDL,SDM,
    SDR,__,SDL,SDM,SDR,__,SDL,SDM,SDR,__,
    __,__,__,__,__,__,__,__,__,__,
    SDL,SDM,SDR,__,SDL,SDM,SDR,__,SDL,SDM,
    SDR,__,SDL,SDM,SDR,__,SDL,SDM,SDR,__,
    SDL,SDM,SDR,__,SDL,SDM,SDR,__,SDL,SDM,
    SDR,__,SDL,SDM,SDR,__,SDL,SDM,SDR,__,
    SDL,SDM,SDR,__,SDL,SDM,SDR,__,SDL,SDM,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r40 */ {
    __,__,__,__,__,__,__,__,__,__,
    VRM,VBL,VWH,__,VRM,VBL,VWH,__,VRM,VBL,
    VWH,__,VRM,VBL,VWH,__,VRM,VBL,VWH,__,
    VRM,VBL,VWH,__,VRM,VBL,VWH,__,VRM,VBL,
    VWH,__,VRM,VBL,VWH,__,VRM,VBL,VWH,__,
    __,__,__,__,__,__,__,__,__,__,
    VRM,VBL,VWH,__,VRM,VBL,VWH,__,VRM,VBL,
    VWH,__,VRM,VBL,VWH,__,VRM,VBL,VWH,__,
    VRM,VBL,VWH,__,VRM,VBL,VWH,__,VRM,VBL,
    VWH,__,VRM,VBL,VWH,__,VRM,VBL,VWH,__,
    VRM,VBL,VWH,__,VRM,VBL,VWH,__,VRM,VBL,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r41 */ { FE },
/* r42 */ { FE },
/* r43 */ { FE },
/* r44 */ { FE },
/* r45 */ { FE },
/* r46 */ { FE },
/* r47 */ { FE },
/* r48 */ { FE },
/* r49 */ { FE },
/* r50 */ { FE },
/* r51 */ { FE },
/* r52 */ { FE },
/* r53 */ { FE },
/* r54 */ { FE },

/* ── RIVER rows 55-59: clear fringe ── */
/* r55 */ { FE },
/* r56 */ { FE },
/* r57 */ { FE },
/* r58 */ { FE },
/* r59 */ { FE },

/* ── AKIHABARA rows 60-79 ── */
/* r60 */ { FE },
/* r61 */ {
    __,__,__,__,__,__,__,__,__,__,
    SDL,SDM,SDR,__,SDL,SDM,SDR,__,SDL,SDM,
    SDR,__,SDL,SDM,SDR,__,SDL,SDM,SDR,__,
    SDL,SDM,SDR,__,SDL,SDM,SDR,__,SDL,SDM,
    SDR,__,SDL,SDM,SDR,__,SDL,SDM,SDR,__,
    __,__,__,__,__,__,__,__,__,__,
    SDL,SDM,SDR,__,SDL,SDM,SDR,__,SDL,SDM,
    SDR,__,SDL,SDM,SDR,__,SDL,SDM,SDR,__,
    SDL,SDM,SDR,__,SDL,SDM,SDR,__,SDL,SDM,
    SDR,__,SDL,SDM,SDR,__,SDL,SDM,SDR,__,
    SDL,SDM,SDR,__,SDL,SDM,SDR,__,SDL,SDM,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r62 */ {
    __,__,__,__,__,__,__,__,__,__,
    VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,
    VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,
    VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,
    VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,
    __,__,__,__,__,__,__,__,__,__,
    VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,
    VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,
    VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,
    VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,
    VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,VRM,VBL,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r63 */ {
    __,__,__,__,__,__,__,__,__,__,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    __,__,__,__,__,__,__,__,__,__,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,WDA,WDB,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r64 */ { FE },
/* r65 */ { FE },
/* r66 */ { FE },
/* r67 */ { FE },
/* r68 */ { FE },
/* r69 */ { FE },
/* r70 */ { FE },
/* r71 */ { FE },
/* r72 */ { FE },
/* r73 */ { FE },
/* r74 */ { FE },
/* r75 */ { FE },
/* r76 */ { FE },
/* r77 */ { FE },
/* r78 */ { FE },
/* r79 */ { FE },

/* ── E-W ROAD BELT #2 rows 80-84 ── */
/* r80 */ { FE },
/* r81 */ { FE },
/* r82 */ { FE },
/* r83 */ { FE },
/* r84 */ { FE },

/* ── UENO PARK rows 85-104 ── */
/* r85 */ { FE },
/* r86 */ {
    __,__,__,__,__,__,__,__,__,__,
    SKT,__,SKT,__,SKT,__,SKT,__,SKT,__,
    SKT,__,SKT,__,SKT,__,SKT,__,SKT,__,
    SKT,__,SKT,__,SKT,__,SKT,__,SKT,__,
    SKT,__,SKT,__,SKT,__,SKT,__,SKT,__,
    __,__,__,__,__,__,__,__,__,__,
    SKT,__,SKT,__,SKT,__,SKT,__,SKT,__,
    SKT,__,SKT,__,SKT,__,SKT,__,SKT,__,
    SKT,__,SKT,__,SKT,__,SKT,__,SKT,__,
    SKT,__,SKT,__,SKT,__,SKT,__,SKT,__,
    SKT,__,SKT,__,SKT,__,SKT,__,SKT,__,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r87 */ {
    __,__,__,__,__,__,__,__,__,__,
    SKB,__,SKB,__,SKB,__,SKB,__,SKB,__,
    SKB,__,SKB,__,SKB,__,SKB,__,SKB,__,
    SKB,__,SKB,__,SKB,__,SKB,__,SKB,__,
    SKB,__,SKB,__,SKB,__,SKB,__,SKB,__,
    __,__,__,__,__,__,__,__,__,__,
    SKB,__,SKB,__,SKB,__,SKB,__,SKB,__,
    SKB,__,SKB,__,SKB,__,SKB,__,SKB,__,
    SKB,__,SKB,__,SKB,__,SKB,__,SKB,__,
    SKB,__,SKB,__,SKB,__,SKB,__,SKB,__,
    SKB,__,SKB,__,SKB,__,SKB,__,SKB,__,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r88 */ { FE },
/* r89 */ { FE },
/* r90 */ { FE },
/* r91 */ { FE },
/* r92 */ { FE },
/* r93 */ { FE },
/* r94 */ { FE },
/* r95 */ { FE },
/* r96 */ { FE },
/* r97 */ { FE },
/* r98 */ { FE },
/* r99 */ { FE },
/* r100 */ { FE },
/* r101 */ { FE },
/* r102 */ { FE },
/* r103 */ { FE },
/* r104 */ { FE },

/* ── ASAKUSA rows 105-117 ── */
/* r105 */ { FE },
/* r106: torii gate row */
/* r106 */ {
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    TRL,TRB,TRR,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    TRL,TRB,TRR,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r107: traditional building walls */
/* r107 */ {
    __,__,__,__,__,__,__,__,__,__,
    TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,
    TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,
    TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,
    TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,
    __,__,__,__,__,__,__,__,__,__,
    TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,
    TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,
    TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,
    TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,
    TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,TRD,TRE,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r108 */ {
    __,__,__,__,__,__,__,__,__,__,
    TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,
    TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,
    TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,
    TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,
    __,__,__,__,__,__,__,__,__,__,
    TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,
    TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,
    TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,
    TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,
    TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,TRA,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r109 */ { FE },
/* r110 */ { FE },
/* r111 */ { FE },
/* r112 */ { FE },
/* r113 */ { FE },
/* r114 */ { FE },
/* r115 */ { FE },
/* r116 */ { FE },
/* r117 */ { FE },
/* r118 */ { FE },
/* r119 */ { FE },
};

/*===========================================================================
 * OVERHEAD DATA  (rooftops, sakura canopies – renders above entire player)
 *===========================================================================*/
static const int16_t g_overheadData[SAMPLE_MAP_H][SAMPLE_MAP_W] = {

/* r0-r1 */ { FE },
/* r1   */ { FE },
/* r2   */ { FE },
/* r3   */ { FE },
/* r4   */ { FE },
/* r5   */ { FE },
/* r6   */ { FE },
/* r7   */ { FE },
/* r8   */ { FE },
/* r9   */ { FE },
/* r10  */ { FE },

/* r11: rooftop edges above r10 2F building facades */
/* r11  */ {
    __,__,__,__,__,__,__,__,__,__,
    RNW,RPN,RNE,__,RNW,RPN,RNE,__,RNW,RPN,
    RNE,__,RNW,RPN,RNE,__,RNW,RPN,RNE,__,
    RNW,RPN,RNE,__,RNW,RPN,RNE,__,RNW,RPN,
    RNE,__,RNW,RPN,RNE,__,RNW,RPN,RNE,__,
    __,__,__,__,__,__,__,__,__,__,
    RNW,RPN,RNE,__,RNW,RPN,RNE,__,RNW,RPN,
    RNE,__,RNW,RPN,RNE,__,RNW,RPN,RNE,__,
    RNW,RPN,RNE,__,RNW,RPN,RNE,__,RNW,RPN,
    RNE,__,RNW,RPN,RNE,__,RNW,RPN,RNE,__,
    RNW,RPN,RNE,__,RNW,RPN,RNE,__,RNW,RPN,
    __,__,__,__,__,__,
    __,__,__,__
},
/* r12  */ { FE },
/* r13  */ { FE },

/* r14: sakura canopy above r14 trunks */
/* r14  */ {
    __,__,__,__,__,__,__,__,__,__,
    SKA,__,__,SKA,__,__,SKA,__,__,SKA,
    __,__,SKA,__,__,SKA,__,__,SKA,__,
    __,SKA,__,__,SKA,__,__,SKA,__,__,
    SKA,__,__,SKA,__,__,SKA,__,__,SKA,
    __,__,__,__,__,__,__,__,__,__,
    SKA,__,__,SKA,__,__,SKA,__,__,SKA,
    __,__,SKA,__,__,SKA,__,__,SKA,__,
    __,SKA,__,__,SKA,__,__,SKA,__,__,
    SKA,__,__,SKA,__,__,SKA,__,__,SKA,
    __,__,SKA,__,__,SKA,__,__,SKA,__,
    __,__,__,__,__,__,
    __,__,__,__
},

/* r15: sakura canopy C */
/* r15  */ {
    __,__,__,__,__,__,__,__,__,__,
    SKC,__,__,SKC,__,__,SKC,__,__,SKC,
    __,__,SKC,__,__,SKC,__,__,SKC,__,
    __,SKC,__,__,SKC,__,__,SKC,__,__,
    SKC,__,__,SKC,__,__,SKC,__,__,SKC,
    __,__,__,__,__,__,__,__,__,__,
    SKC,__,__,SKC,__,__,SKC,__,__,SKC,
    __,__,SKC,__,__,SKC,__,__,SKC,__,
    __,SKC,__,__,SKC,__,__,SKC,__,__,
    SKC,__,__,SKC,__,__,SKC,__,__,SKC,
    __,__,SKC,__,__,SKC,__,__,SKC,__,
    __,__,__,__,__,__,
    __,__,__,__
},

/* r16-r35: clear overhead */
/* r16  */ { FE },
/* r17  */ { FE },
/* r18  */ { FE },
/* r19  */ { FE },
/* r20  */ { FE },
/* r21  */ { FE },
/* r22  */ { FE },
/* r23  */ { FE },
/* r24  */ { FE },
/* r25  */ { FE },
/* r26  */ { FE },
/* r27  */ { FE },
/* r28  */ { FE },
/* r29  */ { FE },
/* r30  */ { FE },
/* r31  */ { FE },
/* r32  */ { FE },
/* r33  */ { FE },
/* r34  */ { FE },
/* r35  */ { FE },
/* r36  */ { FE },

/* r37: Shinjuku roof ridge above r37 2F facades */
/* r37  */ {
    __,__,__,__,__,__,__,__,__,__,
    RNW,RPN,RNE,__,RNW,RPN,RNE,__,RNW,RPN,
    RNE,__,RNW,RPN,RNE,__,RNW,RPN,RNE,__,
    RNW,RPN,RNE,__,RNW,RPN,RNE,__,RNW,RPN,
    RNE,__,RNW,RPN,RNE,__,RNW,RPN,RNE,__,
    __,__,__,__,__,__,__,__,__,__,
    RNW,RPN,RNE,__,RNW,RPN,RNE,__,RNW,RPN,
    RNE,__,RNW,RPN,RNE,__,RNW,RPN,RNE,__,
    RNW,RPN,RNE,__,RNW,RPN,RNE,__,RNW,RPN,
    RNE,__,RNW,RPN,RNE,__,RNW,RPN,RNE,__,
    RNW,RPN,RNE,__,RNW,RPN,RNE,__,RNW,RPN,
    __,__,__,__,__,__,
    __,__,__,__
},

/* r38-r87: clear overhead */
/* r38  */ { FE },
/* r39  */ { FE },
/* r40  */ { FE },
/* r41  */ { FE },
/* r42  */ { FE },
/* r43  */ { FE },
/* r44  */ { FE },
/* r45  */ { FE },
/* r46  */ { FE },
/* r47  */ { FE },
/* r48  */ { FE },
/* r49  */ { FE },
/* r50  */ { FE },
/* r51  */ { FE },
/* r52  */ { FE },
/* r53  */ { FE },
/* r54  */ { FE },
/* r55  */ { FE },
/* r56  */ { FE },
/* r57  */ { FE },
/* r58  */ { FE },
/* r59  */ { FE },
/* r60  */ { FE },
/* r61  */ { FE },
/* r62  */ { FE },
/* r63  */ { FE },
/* r64  */ { FE },
/* r65  */ { FE },
/* r66  */ { FE },
/* r67  */ { FE },
/* r68  */ { FE },
/* r69  */ { FE },
/* r70  */ { FE },
/* r71  */ { FE },
/* r72  */ { FE },
/* r73  */ { FE },
/* r74  */ { FE },
/* r75  */ { FE },
/* r76  */ { FE },
/* r77  */ { FE },
/* r78  */ { FE },
/* r79  */ { FE },
/* r80  */ { FE },
/* r81  */ { FE },
/* r82  */ { FE },
/* r83  */ { FE },
/* r84  */ { FE },
/* r85  */ { FE },
/* r86  */ { FE },

/* r87: Ueno sakura canopy A above r86 trunks */
/* r87  */ {
    __,__,__,__,__,__,__,__,__,__,
    SKA,__,SKA,__,SKA,__,SKA,__,SKA,__,
    SKA,__,SKA,__,SKA,__,SKA,__,SKA,__,
    SKA,__,SKA,__,SKA,__,SKA,__,SKA,__,
    SKA,__,SKA,__,SKA,__,SKA,__,SKA,__,
    __,__,__,__,__,__,__,__,__,__,
    SKA,__,SKA,__,SKA,__,SKA,__,SKA,__,
    SKA,__,SKA,__,SKA,__,SKA,__,SKA,__,
    SKA,__,SKA,__,SKA,__,SKA,__,SKA,__,
    SKA,__,SKA,__,SKA,__,SKA,__,SKA,__,
    SKA,__,SKA,__,SKA,__,SKA,__,SKA,__,
    __,__,__,__,__,__,
    __,__,__,__
},

/* r88-r107: Asakusa trad roofs */
/* r88  */ { FE },
/* r89  */ { FE },
/* r90  */ { FE },
/* r91  */ { FE },
/* r92  */ { FE },
/* r93  */ { FE },
/* r94  */ { FE },
/* r95  */ { FE },
/* r96  */ { FE },
/* r97  */ { FE },
/* r98  */ { FE },
/* r99  */ { FE },
/* r100 */ { FE },
/* r101 */ { FE },
/* r102 */ { FE },
/* r103 */ { FE },
/* r104 */ { FE },
/* r105 */ { FE },
/* r106 */ { FE },
/* r107 */ { FE },
/* r108 */ { FE },
/* r109 */ { FE },
/* r110 */ { FE },
/* r111 */ { FE },
/* r112 */ { FE },
/* r113 */ { FE },
/* r114 */ { FE },
/* r115 */ { FE },
/* r116 */ { FE },
/* r117 */ { FE },
/* r118 */ { FE },
/* r119 */ { FE },
};

/* ── Macro cleanup ────────────────────────────────────────────────────────── */
#undef __
#undef SW
#undef SWB
#undef SWC
#undef SWD
#undef RDA
#undef RDB
#undef RDC
#undef RDD
#undef RDS
#undef RDK
#undef CWA
#undef CWB
#undef CWC
#undef RM
#undef WAT
#undef WTB
#undef WTC
#undef WDA
#undef WDB
#undef WBA
#undef WBB
#undef WAC
#undef WWN
#undef LMP
#undef LMH
#undef VRM
#undef VBL
#undef VWH
#undef ATM
#undef TRC
#undef BRL
#undef SDL
#undef SDM
#undef SDR
#undef NRL
#undef NRR
#undef TRL
#undef TRR
#undef TRB
#undef BF2
#undef BF3
#undef BF4
#undef BF5
#undef BF6
#undef BF7
#undef SKT
#undef SKB
#undef SKA
#undef SKC
#undef TRA
#undef TRD
#undef TRE
#undef RNW
#undef RPN
#undef RNE
#undef RPM
#undef _O
#undef _S
#undef FE
#undef BORDER_ROW
#undef DIST_SW_ROW
#undef DIST_OP_ROW
#undef ROAD_FULL_ROW
#undef ROAD_MARK_ROW
#undef RIVER_ROW
#undef CFLAG_BORDER
#undef CFLAG_ROAD
#undef CFLAG_DIST

/* ── Builder function ─────────────────────────────────────────────────────── */
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
