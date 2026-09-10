#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "json_parser.h"
#include"headerfiles/mapcollision.c"

#define TILE_SIZE 32
#define PLAYER_SIZE 24

typedef struct {
    float x, y;
    float velX, velY;
    float speed;
} Player;

// Custom draw function with scaling
static void DrawTileScaled(Texture2D tileset, int tileID, float screenX, float screenY, float scale) {
    if (tileID == 0) return;
    
    int tilesPerRow = tileset.width / TILE_SIZE;
    int id = tileID - 1;
    int srcX = (id % tilesPerRow) * TILE_SIZE;
    int srcY = (id / tilesPerRow) * TILE_SIZE;
    
    Rectangle source = {srcX, srcY, TILE_SIZE, TILE_SIZE};
    Rectangle dest = {screenX, screenY, TILE_SIZE * scale, TILE_SIZE * scale};
    
    DrawTexturePro(tileset, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
}

static void DrawMapLayerScaled(Texture2D tileset, TileMap *map, int layerIndex, float offsetX, float offsetY, float scale) {
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

// Update player movement with collision
static void UpdatePlayer(Player *player, TileMap *map, int collisionLayer) {
    player->velX = 0;
    player->velY = 0;
    
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        player->velX = -player->speed;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        player->velX = player->speed;
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        player->velY = -player->speed;
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        player->velY = player->speed;
    }
    
    // Try horizontal movement
    float newX = player->x + player->velX;
    if (!CheckCollision(map, collisionLayer, newX, player->y, PLAYER_SIZE)) {
        player->x = newX;
    }
    
    // Try vertical movement
    float newY = player->y + player->velY;
    if (!CheckCollision(map, collisionLayer, player->x, newY, PLAYER_SIZE)) {
        player->y = newY;
    }
    
    // Clamp to map bounds
    int mapPixelWidth = map->width * TILE_SIZE;
    int mapPixelHeight = map->height * TILE_SIZE;
    
    if (player->x < 0) player->x = 0;
    if (player->y < 0) player->y = 0;
    if (player->x + PLAYER_SIZE > mapPixelWidth) player->x = mapPixelWidth - PLAYER_SIZE;
    if (player->y + PLAYER_SIZE > mapPixelHeight) player->y = mapPixelHeight - PLAYER_SIZE;
}

int main() {
    // Initialize window
    int screenWidth = 1200;
    int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Jujutsu Kaisen RPG - Map Viewer");
    SetTargetFPS(60);
    
    // Load map
    GameData game = {0};
    game.map = LoadTileMap("assets/mymap.tmj");
    
    // Calculate scale so the entire map fits on the screen
    float mapPixelWidth = game.map->width * TILE_SIZE;
    float mapPixelHeight = game.map->height * TILE_SIZE;

    float scaleX = (float)screenWidth / mapPixelWidth;
    float scaleY = (float)screenHeight / mapPixelHeight;

    // Use the smaller scale so the whole map fits
    float mapScale = fminf(scaleX, scaleY);

    // Center the map on the screen
    float mapDrawWidth = mapPixelWidth * mapScale;
    float mapDrawHeight = mapPixelHeight * mapScale;

    float mapOffsetX = (screenWidth - mapDrawWidth) / 2.0f;
    float mapOffsetY = (screenHeight - mapDrawHeight) / 2.0f;
    
    if (!game.map) {
        printf("Failed to load map!\n");
        CloseWindow();
        return 1;
    }
    
    // Load tileset texture
    game.tileset = LoadTexture("assets/map.png");
    
    // Find collision layer
    game.collisionLayerIndex = -1;
    for (int i = 0; i < game.map->layerCount; i++) {
        if (strcmp(game.map->layers[i].name, "collision layer") == 0) {
            game.collisionLayerIndex = i;
            break;
        }
    }
    
    if (game.collisionLayerIndex == -1) {
        printf("Warning: Collision layer not found\n");
    }
    
    printf("Map loaded: %dx%d tiles (%dx%d pixels)\n", 
           game.map->width, game.map->height,
           game.map->width * TILE_SIZE, game.map->height * TILE_SIZE);
    printf("Layers: %d\n", game.map->layerCount);
    printf("Map Scale: %.2f, Offset: (%.1f, %.1f)\n", mapScale, mapOffsetX, mapOffsetY);
    for (int i = 0; i < game.map->layerCount; i++) {
        printf("  Layer %d: %s\n", i, game.map->layers[i].name);
    }
    
    // Initialize player
    Player player = {0};
    player.x = 100;
    player.y = 100;
    player.speed = 4.0f;
    
    // Main loop
    while (!WindowShouldClose()) {
        // Update
        UpdatePlayer(&player, game.map, game.collisionLayerIndex);
        
        // Render
        BeginDrawing();
        ClearBackground((Color){20, 20, 30, 255});
        
        // Draw all layers (skip collision layer) with scaling
        for (int i = 0; i < game.map->layerCount; i++) {
            if (i != game.collisionLayerIndex) {
                DrawMapLayerScaled(game.tileset, game.map, i, mapOffsetX, mapOffsetY, mapScale);
            }
        }
        
        // Draw player as a rectangle (scaled)
        float playerScreenX = player.x * mapScale + mapOffsetX;
        float playerScreenY = player.y * mapScale + mapOffsetY;
        float playerScreenSize = PLAYER_SIZE * mapScale;
        
        DrawRectangle((int)playerScreenX, (int)playerScreenY, (int)playerScreenSize, (int)playerScreenSize, RED);
        DrawRectangleLines((int)playerScreenX, (int)playerScreenY, (int)playerScreenSize, (int)playerScreenSize, RED);
        
        // Optional: Draw collision layer as semi-transparent overlay for debugging
        if (IsKeyDown(KEY_C) && game.collisionLayerIndex != -1) {
            TileLayer *collisionLayer = &game.map->layers[game.collisionLayerIndex];
            for (int y = 0; y < collisionLayer->height; y++) {
                for (int x = 0; x < collisionLayer->width; x++) {
                    int index = y * collisionLayer->width + x;
                    if (collisionLayer->data[index] != 0) {
                        float tileX = x * TILE_SIZE * mapScale + mapOffsetX;
                        float tileY = y * TILE_SIZE * mapScale + mapOffsetY;
                        Rectangle tile = {tileX, tileY, TILE_SIZE * mapScale, TILE_SIZE * mapScale};
                        DrawRectangleRec(tile, (Color){255, 0, 0, 50});
                    }
                }
            }
        }
        
        // Draw UI
        DrawText("Jujutsu Kaisen RPG Map", 10, 10, 20, WHITE);
        DrawText(TextFormat("Player: (%.0f, %.0f)", player.x, player.y), 10, 35, 16, WHITE);
        DrawText(TextFormat("Tile: (%d, %d)", (int)(player.x/TILE_SIZE), (int)(player.y/TILE_SIZE)), 10, 55, 16, WHITE);
        DrawText("Controls: WASD/Arrows to move, C to toggle collision view", 10, 75, 14, LIGHTGRAY);
        DrawText(TextFormat("Map: %dx%d | Scale: %.2f", game.map->width, game.map->height, mapScale), 10, 95, 14, LIGHTGRAY);
        
        EndDrawing();
    }
    
    // Cleanup
    UnloadTexture(game.tileset);
    UnloadTileMap(game.map);
    CloseWindow();
    
    return 0;
}