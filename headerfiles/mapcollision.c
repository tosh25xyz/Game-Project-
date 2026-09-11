#include "json_parser.h"
#include "raylib.h"
#include "stdio.h"
#include "mapcollision.h"

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






// Returns true if the tile at (tileX, tileY) on the encounter layer is
// painted (non-zero). Anywhere this layer is painted counts as "inside
// the region" - that's the whole zone, not a single point.
static bool IsInEncounterZone(TileMap *map, int encounterLayer, int tileX, int tileY) {
    return GetTileAtMapPos(map, encounterLayer, tileX, tileY) != 0;
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
// void collisionfunc(Vector2 *nextPos,Vector2 *position,int frameWidth,int frameHeight,int tilesize)
// {
//     float scale = 3.0f;
//     float playerWidth = frameWidth / scale;
//     float playerHeight = frameHeight / scale;
//     int leftTile   = nextPos->x / (tilesize);
//     int rightTile  = (nextPos->x + playerWidth - 1) / (tilesize);
//     int topTile    = nextPos->y / (tilesize);
//     int bottomTile = (nextPos->y + playerHeight - 1) / (tilesize);
//     if (!IsBlocked(leftTile, topTile) &&
//     !IsBlocked(rightTile, topTile) &&
//     !IsBlocked(leftTile, bottomTile) &&
//     !IsBlocked(rightTile, bottomTile))
// {
//     *position = *nextPos;
// }