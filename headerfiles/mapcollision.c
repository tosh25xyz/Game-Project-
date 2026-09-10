#include"json_parser.h"
#include "raylib.h"
#include "stdio.h"
#include"mapcollision.h"


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
static void DrawTile(Texture2D tileset, int tileID, float screenX, float screenY) {
    if (tileID == 0) return;
    
    // Tileset calculation - assumes tileset is organized in a grid
    // Standard Tiled format: tiles are stored in rows
    int tilesPerRow = tileset.width / TILE_SIZE;
    
    // TileID is 1-based, convert to 0-based
    int id = tileID - 1;
    int srcX = (id % tilesPerRow) * TILE_SIZE;
    int srcY = (id / tilesPerRow) * TILE_SIZE;
    
    Rectangle source = {srcX, srcY, TILE_SIZE, TILE_SIZE};
    Rectangle dest = {screenX, screenY, TILE_SIZE, TILE_SIZE};
    
    DrawTexturePro(tileset, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
}
static void DrawMapLayer(Texture2D tileset, TileMap *map, int layerIndex) {
    if (layerIndex >= map->layerCount) return;
    
    TileLayer *layer = &map->layers[layerIndex];
    
    for (int y = 0; y < layer->height; y++) {
        for (int x = 0; x < layer->width; x++) {
            int index = y * layer->width + x;
            int tileID = layer->data[index];
            
            if (tileID != 0) {
                float screenX = x * TILE_SIZE;
                float screenY = y * TILE_SIZE;
                DrawTile(tileset, tileID, screenX, screenY);
            }
        }
    }
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
            if (tileID != 0) {  // 1 means collision
                return true;  // Collision detected
            }
        }
    }
    
    return false;  // No collision
}