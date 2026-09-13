#ifndef MAPCOLLISION_H
#define MAPCOLLISION_H

#include <raylib.h>
#include <stdio.h>
#include "json_parser.h"



// Name of the Tile Layer in Tiled that marks the region where random
// enemy encounters can happen. Paint the whole region with any tile(s)
// on this layer - any non-zero tile here counts as "inside the zone".
// (Your map already has a layer called "enemy" - reuse it as the region.)
#define ENCOUNTER_LAYER_NAME "enemy"

// Chance (out of 100) that moving onto a NEW tile inside the zone
// triggers "A random Beast Appeared!"
#define ENCOUNTER_CHANCE 15

// Name of the Tile Layer in the overworld map that leads INTO the cave.
// Any non-zero tile painted on this layer is a doorway tile.
#define TELEPORT_LAYER_NAME "inside"

// Name of the Tile Layer you add in the destination map (insideCave.tmj)
// that leads back OUT to the overworld. Same idea as TELEPORT_LAYER_NAME,
// just the reverse direction.
#define CAVE_EXIT_LAYER_NAME "exit"
#define HEAL_POINT "heal"
typedef struct {
    Texture2D tileset;
    TileMap *map;
    int collisionLayerIndex;
    int encounterLayerIndex;
    int teleportLayerIndex;   // "inside" layer - present on the overworld map
    int exitLayerIndex;
    int healLayerIndex;       // "exit" layer - present on the cave map
} GameData;
static Mapport SwitchMap(GameData *game, const char *mapPath, const char *tilesetPath,int screenWidth, int screenHeight);
static int GetTileAtMapPos(TileMap *map, int layer, int x, int y);
int CollisionLayer(TileMap *map);
int EncounterLayer(TileMap *map,const char* layername);
void DrawLayer(TileMap *map,int collisionLayerIndex, int encounterLayerIndex, int teleportLayerIndex, int exitLayerIndex, Texture2D tilset,float offsetX, float offsetY, float scale);
void Drawencounter(bool encounter,char spawnMessage[],int screenWidth);
static bool CheckCollision(TileMap *map, int collisionLayer, float x, float y, float size);
static bool IsInEncounterZone(TileMap *map, int encounterLayer, int tileX, int tileY);
void ResolveMovementCollision(TileMap *map, int collisionLayerIndex, Vector2 *position, Vector2 nextPos, float size);
void UpdateEncounterCheck(int curTileX,int curTileY, TileMap *map, int encounterLayerIndex,int *lastTileX, int *lastTileY,char *spawnMessage, bool *encounter);
void UpdateEncounterBoss(int curTileX,int curTileY, TileMap *map, int encounterLayerIndex,int *lastTileX, int *lastTileY,char *spawnZARIF, bool *encounter);
#endif
