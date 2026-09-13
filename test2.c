#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "json_parser.h"
#include "headerfiles/mapcollision.c"
#include"headerfiles/character.c"
#include"headerfiles/tiled.c"

#define PLAYER_SIZE 28
#define SPAWN_MESSAGE_DURATION 2.0f

// Custom draw function with scaling


int main() {
    // Initialize window
    int screenWidth = 1200;
    int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Jujutsu Kaisen RPG - Map Viewer");
    SetTargetFPS(60);
    SetRandomSeed((unsigned int)time(NULL));

    Texture2D texture = LoadTexture("Assets&resources/gojo_matched_size.png");
    int frameWidth = texture.width / 4;
    int frameHeight = texture.height / 4;

    Rectangle frameRec = {0, 0, frameWidth, frameHeight};
    Vector2 position = {256, 96};

    int currentFrame = 0;
    int currentRow = 0; // 0=down,1=left,2=right,3=up

    float frameTime = 0.0f;
    float frameSpeed = 0.15f;

    // Load map
    GameData game = {0};
    game.map = LoadTileMap("assets/NEWMAP.tmj");

    if (!game.map) {
        printf("Failed to load map!\n");
        CloseWindow();
        return 1;
    }

    // Movement + collision always work in this raw, UNSCALED pixel space -
    // it matches the Tiled tile grid exactly. Only drawing gets scaled below.
    Mapport info= CalculateMapviewport(game.map->width,game.map->height,TILE_SIZE,screenWidth,screenHeight);
   

    // Load tileset texture
    game.tileset = LoadTexture("assets/map.png");

    // Find collision layer
    game.collisionLayerIndex = CollisionLayer(game.map);
    

    

    
    game.encounterLayerIndex=EncounterLayer(game.map,ENCOUNTER_LAYER_NAME);
    
    printf("Map loaded: %dx%d tiles (%dx%d pixels)\n",game.map->width, game.map->height,game.map->width * TILE_SIZE, game.map->height * TILE_SIZE);
    printf("Layers: %d\n", game.map->layerCount);
    printf("Map Scale: %.2f, Offset: (%.1f, %.1f)\n", info.scale, info.offsetX, info.offsetY);
    for (int i = 0; i < game.map->layerCount; i++) {
        printf("  Layer %d: %s\n", i, game.map->layers[i].name);
    }

    bool moving = false;

    // Tracks the player's last tile so the encounter roll only fires
    // once per NEW tile entered - not every single frame.
    int lastTileX = (int)(position.x / TILE_SIZE);
    int lastTileY = (int)(position.y / TILE_SIZE);

    // "A random Beast Appeared!" popup text + fade timer
    char spawnMessage[64] = "";
    //float spawnMessageTimer = 0.0f;
    bool encounter=false;
    // Main loop
    while (!WindowShouldClose()) {
        // Update
        Vector2 nextPos = position;
        moving = false;
        charactermovement(&nextPos, &currentRow, frameWidth, frameHeight, TILE_SIZE, &moving);

        // Gate each axis through the same tile collision mapcollision.c already
        // does for the old Player struct - move only if that axis is clear
       ResolveMovementCollision(game.map, game.collisionLayerIndex, &position, nextPos, PLAYER_SIZE);

        // Clamp to map bounds (map-based, not clampcharacter's screen-based clamp)
        ClampPositionToMap(&position, PLAYER_SIZE, info.pixelWidth, info.pixelHeight);
        

        if (moving) {
            animation(&frameTime, &currentFrame, frameSpeed);
        }
        UpdateFrame(&frameRec, &currentFrame, &currentRow, frameWidth, frameHeight);

            int curTileX = (int)(position.x / TILE_SIZE);
            int curTileY = (int)(position.y / TILE_SIZE);
        // ---- Random encounter check (Pokemon-style tall grass) ----
        UpdateEncounterCheck(curTileX,curTileY, game.map, game.encounterLayerIndex,&lastTileX, &lastTileY, spawnMessage, &encounter);
        int tileUnderPlayer = GetTileAtMapPos(game.map, game.collisionLayerIndex, curTileX, curTileY);
        // Render
        BeginDrawing();
        ClearBackground((Color){20, 20, 30, 255});

        // Draw all layers (skip collision + encounter zone layers) with scaling
        DrawLayer(game.map,game.collisionLayerIndex,game.encounterLayerIndex,game.tileset, info.offsetX, info.offsetY, info.scale);
        DrawCharacter(texture, frameRec, position, frameWidth, frameHeight, info.scale, info.offsetX, info.offsetY);

        // Optional: Draw collision layer as semi-transparent overlay for debugging
        if (IsKeyDown(KEY_C) && game.collisionLayerIndex != -1) {
            TileLayer *collisionLayer = &game.map->layers[game.collisionLayerIndex];
            for (int y = 0; y < collisionLayer->height; y++) {
                for (int x = 0; x < collisionLayer->width; x++) {
                    int index = y * collisionLayer->width + x;
                    if (collisionLayer->data[index] != 0) {
                        float tileX = x * TILE_SIZE * info.scale + info.offsetX;
                        float tileY = y * TILE_SIZE * info.scale + info.offsetY;
                        Rectangle tile = {tileX, tileY, TILE_SIZE * info.scale, TILE_SIZE * info.scale};
                        DrawRectangleRec(tile, (Color){255, 0, 0, 50});
                    }
                }
            }
        }

        // "A random Beast Appeared!" message, fades out after SPAWN_MESSAGE_DURATION seconds
        Drawencounter(encounter,spawnMessage,screenWidth);
        
        // Draw UI
        DrawDebugUI(position, nextPos, moving,game.map->width, game.map->height, info.scale,game.collisionLayerIndex, tileUnderPlayer);
        EndDrawing();
    }

    // Cleanup
    UnloadTexture(game.tileset);
    UnloadTileMap(game.map);
    CloseWindow();

    return 0;
}