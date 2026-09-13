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
#include"headerfiles/gamemode.h"

#define PLAYER_SIZE 28
#define SPAWN_MESSAGE_DURATION 2.0f

// ---- Map transition config ----
// TELEPORT_LAYER_NAME and CAVE_EXIT_LAYER_NAME come from mapcollision.h
#define OVERWORLD_MAP_PATH   "assets/NEWMAP.tmj"
#define OVERWORLD_TILESET    "assets/map.png"
#define CAVE_MAP_PATH        "assets/insideCave.tmj"
#define CAVE_TILESET         "assets/map.png"   // change if the cave uses a different tileset image

// Known-open landing tiles (checked against each map's collision layer)
#define CAVE_SPAWN_TILE_X      14
#define CAVE_SPAWN_TILE_Y      6
#define OVERWORLD_SPAWN_TILE_X 11
#define OVERWORLD_SPAWN_TILE_Y 7



// Custom draw function with scaling

// Unloads the current map/tileset and loads a new one in their place,
// recomputing every layer index and the viewport for the new map.


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
    game.map = LoadTileMap(OVERWORLD_MAP_PATH);

    if (!game.map) {
        printf("Failed to load map!\n");
        CloseWindow();
        return 1;
    }

   GameMode currentMap = MODE_OVERWORLD;

    // Movement + collision always work in this raw, UNSCALED pixel space -
    // it matches the Tiled tile grid exactly. Only drawing gets scaled below.
    Mapport info = CalculateMapviewport(game.map->width, game.map->height, TILE_SIZE, screenWidth, screenHeight);

    // Load tileset texture
    game.tileset = LoadTexture(OVERWORLD_TILESET);

    // Find collision / encounter / teleport / exit layers
    game.collisionLayerIndex = CollisionLayer(game.map);
    game.encounterLayerIndex = EncounterLayer(game.map, ENCOUNTER_LAYER_NAME);
    game.teleportLayerIndex  = EncounterLayer(game.map, TELEPORT_LAYER_NAME);
    game.exitLayerIndex      = EncounterLayer(game.map, CAVE_EXIT_LAYER_NAME); // -1 on the overworld map, that's fine

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

    // Edge-trigger guards so stepping onto (and standing on) a teleport
    // tile only fires the map switch once, not every frame.
    bool wasOnTeleportTile = false;
    bool wasOnExitTile = false;

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

        // ---- Map transition checks ----
        if (currentMap == MODE_OVERWORLD && game.teleportLayerIndex != -1) {
            bool onTeleportTile = (GetTileAtMapPos(game.map, game.teleportLayerIndex, curTileX, curTileY) != 0);
            if (onTeleportTile && !wasOnTeleportTile) {
                info = SwitchMap(&game, CAVE_MAP_PATH, CAVE_TILESET, screenWidth, screenHeight);
                
                position = (Vector2){ 452,202 };
                lastTileX = CAVE_SPAWN_TILE_X;
                lastTileY = CAVE_SPAWN_TILE_Y;
                wasOnTeleportTile = false;
                wasOnExitTile = false;
                encounter = false;
                spawnMessage[0] = '\0';
                currentMap=MODE_CAVE;
                continue; // skip the rest of this frame - we just swapped maps out from under it
            }
            wasOnTeleportTile = onTeleportTile;
        } else if (currentMap == MODE_CAVE) {
            // Falls back to a key press if you haven't added the "exit" layer to insideCave.tmj yet.
            bool onExitTile = false;
            if (game.exitLayerIndex != -1) {
                onExitTile = GetTileAtMapPos(game.map, game.exitLayerIndex, curTileX, curTileY) != 0;
            }
            bool exitPressed = (game.exitLayerIndex == -1) && IsKeyPressed(KEY_BACKSPACE);

            if ((onExitTile && !wasOnExitTile) || exitPressed) {
                info = SwitchMap(&game, OVERWORLD_MAP_PATH, OVERWORLD_TILESET, screenWidth, screenHeight);
                
                position = (Vector2){ 482,74 };
                lastTileX = OVERWORLD_SPAWN_TILE_X;
                lastTileY = OVERWORLD_SPAWN_TILE_Y;
                wasOnTeleportTile = false;
                wasOnExitTile = false;
                encounter = false;
                spawnMessage[0] = '\0';
                currentMap = MODE_OVERWORLD;
                continue;
            }
            wasOnExitTile = onExitTile;
        }

        // ---- Random encounter check (Pokemon-style tall grass) ----
        // Only rolls encounters in the overworld - remove this guard if the cave should have its own.
        if (currentMap == MODE_OVERWORLD) {
            UpdateEncounterCheck(curTileX,curTileY, game.map, game.encounterLayerIndex,&lastTileX, &lastTileY, spawnMessage, &encounter);
        }
        else if(currentMap==MODE_CAVE)
        {
            UpdateEncounterBoss(curTileX,curTileY, game.map, game.encounterLayerIndex,&lastTileX, &lastTileY, spawnMessage, &encounter);
        }
        int tileUnderPlayer = GetTileAtMapPos(game.map, game.collisionLayerIndex, curTileX, curTileY);
        // Render
        BeginDrawing();
        ClearBackground((Color){20, 20, 30, 255});

        // Draw all layers (skip collision, encounter zone, and teleport/exit marker layers) with scaling
        DrawLayer(game.map,game.collisionLayerIndex,game.encounterLayerIndex,game.teleportLayerIndex,game.exitLayerIndex,game.tileset, info.offsetX, info.offsetY, info.scale);
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