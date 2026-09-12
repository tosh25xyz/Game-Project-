
#ifndef TILED_H
#define TILED_H


#include"raylib.h"
#include"stdio.h"

#define TILE_SIZE 32
typedef struct {
    float pixelWidth;
    float pixelHeight;
    float scale;
    float offsetX;
    float offsetY;
} Mapport;
void DrawTileScaled(Texture2D tileset, int tileID, float screenX, float screenY, float scale);
void DrawLayer(TileMap *map,int collisionLayerIndex, int encounterLayerIndex,Texture2D tilset,float offsetX, float offsetY, float scale);
void DrawMapLayerScaled(Texture2D tileset, TileMap *map, int layerIndex, float offsetX, float offsetY, float scale);
Mapport CalculateMapviewport(int mapWidth, int mapHeight, int tileSize,int screenWidth, int screenHeight);
void DrawDebugUI(Vector2 position, Vector2 nextPos, bool moving,int mapWidth, int mapHeight, float scale,int collisionLayerIndex, int tileUnderPlayer);
#endif 