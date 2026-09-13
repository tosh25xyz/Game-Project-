#include "json_parser.h"
#include "raylib.h"
#include "stdio.h"
#include "mapcollision.h"
#include"tiled.h"

static int GetTileAtMapPos(TileMap *map, int layer, int x, int y) {
    if (x < 0 || x >= map->width || y < 0 || y >= map->height) {
        return 0;
    }
    if (layer < 0 || layer >= map->layerCount) return 0;
    if (!map->layers[layer].data) return 0;

    int index = y * map->width + x;
    if (index >= 0 && index < map->width * map->height) {
        return map->layers[layer].data[index];
    }
    return 0;
}
int CollisionLayer(TileMap *map)
{
    for (int i = 0; i < map->layerCount; i++) {
        if (strcmp(map->layers[i].name, "collision Layer") == 0) {
            return i;
        
        }
    }
    return -1;
}
int EncounterLayer(TileMap *map,const char *layername)
{
    
    for (int i = 0; i < map->layerCount; i++) {
        if (strcmp(map->layers[i].name, layername) == 0) {
            return i;
        }
    }
   return -1;
}






// Returns true if the tile at (tileX, tileY) on the encounter layer is
// painted (non-zero). Anywhere this layer is painted counts as "inside
// the region" - that's the whole zone, not a single point.
static bool IsInEncounterZone(TileMap *map, int encounterLayer, int tileX, int tileY) {
    return GetTileAtMapPos(map, encounterLayer, tileX, tileY) != 0;
}
void UpdateEncounterCheck(int curTileX,int curTileY, TileMap *map, int encounterLayerIndex,int *lastTileX, int *lastTileY,char *spawnMessage, bool *encounter) {


    if (curTileX != *lastTileX || curTileY != *lastTileY) {
        *lastTileX = curTileX;
        *lastTileY = curTileY;

        if (encounterLayerIndex != -1 &&
            IsInEncounterZone(map, encounterLayerIndex, curTileX, curTileY)) {

            if (GetRandomValue(1, 100) <= ENCOUNTER_CHANCE) {
                strcpy(spawnMessage, "A random Beast Appeared!");
                *encounter = true;
            }
        }
    }

    if (*encounter && IsKeyPressed(KEY_ENTER)) {
        *encounter = false;
    }
}
void UpdateEncounterBoss(int curTileX,int curTileY, TileMap *map, int encounterLayerIndex,int *lastTileX, int *lastTileY,char *spawnZARIF, bool *encounter) {


    if (curTileX != *lastTileX || curTileY != *lastTileY) {
        *lastTileX = curTileX;
        *lastTileY = curTileY;

        if (encounterLayerIndex != -1 &&
            IsInEncounterZone(map, encounterLayerIndex, curTileX, curTileY)) {

            if (GetRandomValue(1, 100) <= ENCOUNTER_CHANCE) {
                strcpy(spawnZARIF, "Mysterious ENEMY APPEARED");
                *encounter = true;
            }
        }
    }

    // if (*encounter && IsKeyPressed(KEY_SPACE)) {
    //     *encounter = false;
    // }
}

static bool CheckCollision(TileMap *map, int collisionLayer, float x, float y, float size) {
    // Convert pixel position to tile coordinates
    int tileX = (int)(x / TILE_SIZE);
    int tileY = (int)(y / TILE_SIZE);
    int tileX2 = (int)((x + size) / TILE_SIZE);
    int tileY2 = (int)((y + size) / TILE_SIZE);

    // Check all tiles the character overlaps
    for (int ty = tileY; ty <= tileY2; ty++) {
        for (int tx = tileX; tx <= tileX2; tx++) {
            int tileID = GetTileAtMapPos(map, collisionLayer, tx, ty);
            if (tileID != 0) {  // non-zero means collision
                return true;
            }
        }
    }

    return false;
}
void ResolveMovementCollision(TileMap *map, int collisionLayerIndex,
                               Vector2 *position, Vector2 nextPos, float size) {

    if (!CheckCollision(map, collisionLayerIndex, nextPos.x, position->y, size)) {
        position->x = nextPos.x;
    }
    if (!CheckCollision(map, collisionLayerIndex, position->x, nextPos.y, size)) {
        position->y = nextPos.y;
    }
}

void Drawencounter(bool encounter,char spawnMessage[],int screenWidth)
{
    if (encounter) {
            
            int textWidth = MeasureText(spawnMessage, 24);
            DrawText(spawnMessage, (screenWidth - textWidth) / 2, 150, 24, YELLOW);
        }
}

static Mapport SwitchMap(GameData *game, const char *mapPath, const char *tilesetPath,
                          int screenWidth, int screenHeight) {
    UnloadTexture(game->tileset);
    UnloadTileMap(game->map);

    game->map = LoadTileMap(mapPath);
    if (!game->map) {
        printf("Failed to load map: %s\n", mapPath);
        CloseWindow();
        exit(1);
    }
    game->tileset = LoadTexture(tilesetPath);

    game->collisionLayerIndex = CollisionLayer(game->map);
    game->encounterLayerIndex = EncounterLayer(game->map, ENCOUNTER_LAYER_NAME);
    game->teleportLayerIndex  = EncounterLayer(game->map, TELEPORT_LAYER_NAME);
    game->exitLayerIndex      = EncounterLayer(game->map, CAVE_EXIT_LAYER_NAME);

    printf("Map loaded: %s (%dx%d tiles)\n", mapPath, game->map->width, game->map->height);

    return CalculateMapviewport(game->map->width, game->map->height, TILE_SIZE, screenWidth, screenHeight);
}