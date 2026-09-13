#include "raylib.h"

#include"tiled.h"
#include"json_parser.h"
// Custom draw function with scaling
void DrawTileScaled(Texture2D tileset, int tileID, float screenX, float screenY, float scale) {
    if (tileID == 0) return;

    int tilesPerRow = tileset.width / TILE_SIZE;
    int id = tileID - 1;
    int srcX = (id % tilesPerRow) * TILE_SIZE;
    int srcY = (id / tilesPerRow) * TILE_SIZE;

    Rectangle source = {srcX, srcY, TILE_SIZE, TILE_SIZE};
    Rectangle dest = {screenX, screenY, TILE_SIZE * scale, TILE_SIZE * scale};

    DrawTexturePro(tileset, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
}

 void DrawMapLayerScaled(Texture2D tileset, TileMap *map, int layerIndex, float offsetX, float offsetY, float scale) {
    if (layerIndex >= map->layerCount) return;

    TileLayer *layer = &map->layers[layerIndex];

    for (int y = 0; y < layer->height; y++) {
        for (int x = 0; x < layer->width; x++) {
            int index = y * layer->width + x;
            int tileID = layer->data[index];

            if (tileID != 0) {
                float screenX = x * TILE_SIZE * scale + offsetX;
                float screenY = y * TILE_SIZE * scale + offsetY;
                DrawTileScaled(tileset, tileID, screenX, screenY, scale);
            }
        }
    }
}
void DrawLayer(TileMap *map,int collisionLayerIndex, int encounterLayerIndex, int teleportLayerIndex, int exitLayerIndex, Texture2D tileset,float offsetX, float offsetY, float scale) {

    for (int i = 0; i < map->layerCount; i++) {
        if (i != collisionLayerIndex && i != encounterLayerIndex &&
            i != teleportLayerIndex && i != exitLayerIndex) {
            DrawMapLayerScaled(tileset, map, i, offsetX, offsetY, scale);
        }
    }
}
Mapport CalculateMapviewport(int mapWidth, int mapHeight, int tileSize,
                                  int screenWidth, int screenHeight) {
    Mapport vp;

    vp.pixelWidth  = mapWidth * tileSize;
    vp.pixelHeight = mapHeight * tileSize;

    float scaleX = (float)screenWidth / vp.pixelWidth;
    float scaleY = (float)screenHeight / vp.pixelHeight;
    vp.scale = fminf(scaleX, scaleY);

    float drawWidth  = vp.pixelWidth * vp.scale;
    float drawHeight = vp.pixelHeight * vp.scale;

    vp.offsetX = (screenWidth - drawWidth) / 2.0f;
    vp.offsetY = (screenHeight - drawHeight) / 2.0f;

    return vp;
}
void DrawDebugUI(Vector2 position, Vector2 nextPos, bool moving,
                  int mapWidth, int mapHeight, float scale,
                  int collisionLayerIndex) {

    DrawText("Jujutsu Kaisen RPG Map", 10, 30, 20, WHITE);
    DrawText(TextFormat("Player: (%.0f, %.0f)", position.x, position.y), 10, 55, 16, WHITE);
    DrawText(TextFormat("Tile: (%d, %d)", (int)(position.x/TILE_SIZE), (int)(position.y/TILE_SIZE)), 10, 75, 16, WHITE);
    DrawText("Controls: WASD/Arrows to move, C to toggle collision view", 10, 95, 14, LIGHTGRAY);
    DrawText(TextFormat("Map: %dx%d | Scale: %.2f", mapWidth, mapHeight, scale), 10, 115, 14, LIGHTGRAY);
    DrawText(TextFormat("Moving: %s | NextPos: (%.1f, %.1f)", moving ? "YES" : "NO", nextPos.x, nextPos.y), 10, 135, 14, GREEN);
    //DrawText(TextFormat("CollisionLayerIdx: %d | TileID under player: %d", collisionLayerIndex, tileUnderPlayer), 10, 135, 14, ORANGE);
}