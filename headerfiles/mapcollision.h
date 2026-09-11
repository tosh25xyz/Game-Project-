#ifndef MAPCOLLISION_H
#define MAPCOLLISION_H

#include <raylib.h>
#include <stdio.h>
#include "json_parser.h"

#define TILE_SIZE 32

// Name of the Tile Layer in Tiled that marks the region where random
// enemy encounters can happen. Paint the whole region with any tile(s)
// on this layer - any non-zero tile here counts as "inside the zone".
// (Your map already has a layer called "enemy" - reuse it as the region.)
#define ENCOUNTER_LAYER_NAME "enemy"

// Chance (out of 100) that moving onto a NEW tile inside the zone
// triggers "A random Beast Appeared!"
#define ENCOUNTER_CHANCE 15

typedef struct {
    Texture2D tileset;
    TileMap *map;
    int collisionLayerIndex;
    int encounterLayerIndex;
} GameData;

static int GetTileAtMapPos(TileMap *map, int layer, int x, int y);
// static void DrawTile(Texture2D tileset, int tileID, float screenX, float screenY);
// static void DrawMapLayer(Texture2D tileset, TileMap *map, int layerIndex);
static bool CheckCollision(TileMap *map, int collisionLayer, float x, float y, float size);
static bool IsInEncounterZone(TileMap *map, int encounterLayer, int tileX, int tileY);

#endif