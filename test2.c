#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "json_parser.h"
#include "headerfiles/mapcollision.c"
#include"headerfiles/character.c"

#define PLAYER_SIZE 16
#define SPAWN_MESSAGE_DURATION 2.0f

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
    float mapPixelWidth = game.map->width * TILE_SIZE;
    float mapPixelHeight = game.map->height * TILE_SIZE;

    // Calculate scale so the entire map fits on the screen
    float scaleX = (float)screenWidth / mapPixelWidth;
    float scaleY = (float)screenHeight / mapPixelHeight;

    // Use the smaller scale so the whole map fits
    float mapScale = fminf(scaleX, scaleY);

    // Center the map on the screen
    float mapDrawWidth = mapPixelWidth * mapScale;
    float mapDrawHeight = mapPixelHeight * mapScale;

    float mapOffsetX = (screenWidth - mapDrawWidth) / 2.0f;
    float mapOffsetY = (screenHeight - mapDrawHeight) / 2.0f;

    // Load tileset texture
    game.tileset = LoadTexture("assets/map.png");

    // Find collision layer
    game.collisionLayerIndex = -1;
    for (int i = 0; i < game.map->layerCount; i++) {
        if (strcmp(game.map->layers[i].name, "collision Layer") == 0) {
            game.collisionLayerIndex = i;
            break;
        }
    }

    if (game.collisionLayerIndex == -1) {
        printf("Warning: Collision layer not found\n");
    }

    // Find the encounter zone layer - anywhere it's painted is the
    // region where random beast encounters can happen while walking.
    game.encounterLayerIndex = -1;
    for (int i = 0; i < game.map->layerCount; i++) {
        if (strcmp(game.map->layers[i].name, ENCOUNTER_LAYER_NAME) == 0) {
            game.encounterLayerIndex = i;
            break;
        }
    }

    if (game.encounterLayerIndex == -1) {
        printf("Warning: Encounter zone layer \"%s\" not found\n", ENCOUNTER_LAYER_NAME);
    }

    printf("Map loaded: %dx%d tiles (%dx%d pixels)\n",
           game.map->width, game.map->height,
           game.map->width * TILE_SIZE, game.map->height * TILE_SIZE);
    printf("Layers: %d\n", game.map->layerCount);
    printf("Map Scale: %.2f, Offset: (%.1f, %.1f)\n", mapScale, mapOffsetX, mapOffsetY);
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
    float spawnMessageTimer = 0.0f;

    // Main loop
    while (!WindowShouldClose()) {
        // Update
        Vector2 nextPos = position;
        moving = false;
        charactermovement(&nextPos, &currentRow, frameWidth, frameHeight, TILE_SIZE, &moving);

        // Gate each axis through the same tile collision mapcollision.c already
        // does for the old Player struct - move only if that axis is clear
        if (!CheckCollision(game.map, game.collisionLayerIndex, nextPos.x, position.y, PLAYER_SIZE)) {
            position.x = nextPos.x;
        }
        if (!CheckCollision(game.map, game.collisionLayerIndex, position.x, nextPos.y, PLAYER_SIZE)) {
            position.y = nextPos.y;
        }

        // Clamp to map bounds (map-based, not clampcharacter's screen-based clamp)
        if (position.x < 0) position.x = 0;
        if (position.y < 0) position.y = 0;
        if (position.x + PLAYER_SIZE > mapPixelWidth) position.x = mapPixelWidth - PLAYER_SIZE;
        if (position.y + PLAYER_SIZE > mapPixelHeight) position.y = mapPixelHeight - PLAYER_SIZE;

        if (moving) {
            animation(&frameTime, &currentFrame, frameSpeed);
        }
        UpdateFrame(&frameRec, &currentFrame, &currentRow, frameWidth, frameHeight);

        // ---- Random encounter check (Pokemon-style tall grass) ----
        int curTileX = (int)(position.x / TILE_SIZE);
        int curTileY = (int)(position.y / TILE_SIZE);
        int tileUnderPlayer = GetTileAtMapPos(game.map, game.collisionLayerIndex, curTileX, curTileY);

        if (curTileX != lastTileX || curTileY != lastTileY) {
            lastTileX = curTileX;
            lastTileY = curTileY;

            if (game.encounterLayerIndex != -1 &&
                IsInEncounterZone(game.map, game.encounterLayerIndex, curTileX, curTileY)) {

                if (GetRandomValue(1, 100) <= ENCOUNTER_CHANCE) {
                    strcpy(spawnMessage, "A random Beast Appeared!");
                    spawnMessageTimer = SPAWN_MESSAGE_DURATION;
                }
            }
        }

        // Render
        BeginDrawing();
        ClearBackground((Color){20, 20, 30, 255});

        // Draw all layers (skip collision + encounter zone layers) with scaling
        for (int i = 0; i < game.map->layerCount; i++) {
            if (i != game.collisionLayerIndex && i != game.encounterLayerIndex) {
                DrawMapLayerScaled(game.tileset, game.map, i, mapOffsetX, mapOffsetY, mapScale);
            }
        }

        DrawCharacter(texture, frameRec, position, frameWidth, frameHeight, mapScale, mapOffsetX, mapOffsetY);

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

        // "A random Beast Appeared!" message, fades out after SPAWN_MESSAGE_DURATION seconds
        if (spawnMessageTimer > 0.0f) {
            spawnMessageTimer -= GetFrameTime();
            int textWidth = MeasureText(spawnMessage, 24);
            DrawText(spawnMessage, (screenWidth - textWidth) / 2, 150, 24, YELLOW);
        }

        // Draw UI
        DrawText("Jujutsu Kaisen RPG Map", 10, 10, 20, WHITE);
        DrawText(TextFormat("Player: (%.0f, %.0f)", position.x, position.y), 10, 35, 16, WHITE);
        DrawText(TextFormat("Tile: (%d, %d)", (int)(position.x/TILE_SIZE), (int)(position.y/TILE_SIZE)), 10, 55, 16, WHITE);
        DrawText("Controls: WASD/Arrows to move, C to toggle collision view", 10, 75, 14, LIGHTGRAY);
        DrawText(TextFormat("Map: %dx%d | Scale: %.2f", game.map->width, game.map->height, mapScale), 10, 95, 14, LIGHTGRAY);
        DrawText(TextFormat("Moving: %s | NextPos: (%.1f, %.1f)", moving ? "YES" : "NO", nextPos.x, nextPos.y), 10, 115, 14, GREEN);
        DrawText(TextFormat("CollisionLayerIdx: %d | TileID under player: %d", game.collisionLayerIndex, tileUnderPlayer), 10, 135, 14, ORANGE);

        EndDrawing();
    }

    // Cleanup
    UnloadTexture(game.tileset);
    UnloadTileMap(game.map);
    CloseWindow();

    return 0;
}