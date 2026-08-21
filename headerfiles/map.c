#include "raylib.h"
#include "map.h"
#include<stdio.h>

// #define TILE_SIZE 32

// Tile selector
// #define TILE(col,row) (Rectangle){col*16,row*16,16,16}

// Map size
// #define MAP_WIDTH 30
// #define MAP_HEIGHT 20


    
void InitTileRects(Rectangle tileRects[])
{
    
    // Tile locations in tileset
    

    tileRects[0] = TILE(0,0);   /// Cave Background
    tileRects[1] = TILE(1, 0);   // grass
    tileRects[2] = TILE(1, 4);   // Mountain
    tileRects[3] = TILE(3, 0);   // wall
    tileRects[4] = TILE(4, 0);   // SEA
    tileRects[5] = TILE(5, 0);   // WATERFALL
    tileRects[6] = TILE(6, 0);   // WHITE STUFF
    tileRects[7] = TILE(7, 0);  // CLEAN POND BOUNDARY
    tileRects[8] = TILE(8, 0);  // cLEAN pond Boundary 2
    tileRects[9] = TILE(9, 0);  // CLEAR PONDS
    tileRects[10] = TILE(10, 0); // SIDE BORDER OF POND
    tileRects[11] = TILE(3, 0);  // ROck
    tileRects[12] = TILE(1, 5);  // DARK CAVE BORDER
    tileRects[13] = TILE(0, 4);  // DARK CAVE BORDER side
    tileRects[14] = TILE(14, 0);  // DARK CAVE BORDER curve
    tileRects[15] = TILE(7, 0);  // DOOR
    tileRects[16] = TILE(10, 5); // sea edge
    tileRects[17] = TILE(11, 5); // corner sea
    tileRects[18] = TILE(10, 6); // middlesea edge
    tileRects[19] = TILE(11, 6); // side sea edge
    tileRects[20] = TILE(20, 0);  // QUICK SAND SIDE EDGE
    tileRects[21] = TILE(10, 1); // upper quick sand edge
    tileRects[22] = TILE(1, 5);  // pahar
    tileRects[23] = TILE(2, 5);  // pahar er kona
    tileRects[24] = TILE(2, 4);  // pahar er  kona
    tileRects[25] = TILE(3, 4);  // onno pahar er side
    tileRects[26] = TILE(3, 5);  // curve of onno pahar
    tileRects[27] = TILE(4, 4);  // onno pahar
    tileRects[28] = TILE(4, 5);  // onno  pahar front side
    tileRects[29] = TILE(0, 3);  // pahar    first curve
    tileRects[30] = TILE(1, 3);  // paharu uperface
    tileRects[31] = TILE(5, 1);  // BUSH
    tileRects[32] = TILE(9, 2);  // QUICK SAND SIDE LAYER
    tileRects[33] = TILE(10, 2); // qUICK SAND MAIN
    tileRects[34] = TILE(5, 0);  // Sign board
    tileRects[35] = TILE(5, 2);  // SMALL CACTUS

    // ELEMENT LOCATION  IN TILESET

    tileRects[36] = TILE(4, 2); // CACTUS LONG
    tileRects[37] = TILE(4, 1); // cactus long
    tileRects[38] = TILE(4, 0); // CACTUS LONG
    tileRects[39] = TILE(1, 2); // LONG TREE
    tileRects[40] = TILE(1, 1); // LONG TREE
    tileRects[41] = TILE(1, 0); // LONG TREEE
    tileRects[42] = TILE(2, 0); // LONG TREE
    tileRects[43] = TILE(2, 1); // LONG TTREE
    tileRects[44] = TILE(2, 2); // LONG TREE
    tileRects[45] = TILE(3, 1); // SMALL TREE
    tileRects[46] = TILE(3, 2); // SMALL TREE
    tileRects[47] = TILE(8, 0); // butterfly
}

void DRAWLAYERFIRST(Texture2D tileset,Rectangle tileRects[],int map[MAP_HEIGHT][MAP_WIDTH],int tilesize)
{
for (int y = 0; y < MAP_HEIGHT; y++)
        {
            for (int x = 0; x < MAP_WIDTH; x++)
            {
                // tile id from map
                int tileID = map[y][x];

                // draw tile
                DrawTexturePro(
                    tileset,

                    // source tile
                    tileRects[tileID],

                    // destination on screen
                    (Rectangle){
                        x * tilesize ,
                        y *  tilesize,

                        tilesize ,
                        tilesize},

                    (Vector2){0, 0},
                    0.0f,
                    WHITE);
            }
        }
}
void DRAWLAYERsecond(Texture2D tileset,Rectangle tileRects[],int basemaps[20][30],int tilesize)
{
    for (int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 30; x++)
            {
                // tile id from map
                int tileID = basemaps[y][x];

                // draw tile
                DrawTexturePro(
                    tileset,

                    // source tile
                    tileRects[tileID],

                    // destination on screen
                    (Rectangle){
                        x * tilesize ,
                        y * tilesize ,

                        tilesize ,
                        tilesize },

                    (Vector2){0, 0},
                    0.0f,
                    WHITE);
            }
        }
}
void DRAWLAYERTHIRD(Texture2D tileset,Rectangle tileRects[],int maps[20][30],int tilesize)
{
    for (int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 30; x++)
            {
                // tile id from map
                int tileID = maps[y][x];

                // draw tile
                DrawTexturePro(
                    tileset,

                    // source tile
                    tileRects[tileID],

                    // destination on screen
                    (Rectangle){
                        x * tilesize ,
                        y * tilesize ,

                        tilesize ,
                        tilesize },

                    (Vector2){0, 0},
                    0.0f,
                    WHITE);
            }
        }
}
void DRAWLAYER4TH(Texture2D tileset,Rectangle tileRects[],int mapoverlap[20][30],int tilesize)
{
    for (int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 30; x++)
            {
                // tile id from map
                int tileID = mapoverlap[y][x];

                // draw tile
                DrawTexturePro(
                    tileset,

                    // source tile
                    tileRects[tileID],

                    // destination on screen
                    (Rectangle){
                        x * tilesize ,
                        y * tilesize ,

                        tilesize ,
                        tilesize },

                    (Vector2){0, 0},
                    0.0f,
                    WHITE);
            }
        }
}