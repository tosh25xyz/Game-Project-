#include <raylib.h>
#include <stdio.h>
#include "json_parser.h"
#define TILE_SIZE 32


typedef struct {
    Texture2D tileset;
    TileMap *map;
    int collisionLayerIndex;
} GameData;

static int GetTileAtMapPos(TileMap *map, int layer, int x, int y);
static void DrawTile(Texture2D tileset, int tileID, float screenX, float screenY);
static void DrawMapLayer(Texture2D tileset, TileMap *map, int layerIndex);
static bool CheckCollision(TileMap *map, int collisionLayer, float x, float y, float size);