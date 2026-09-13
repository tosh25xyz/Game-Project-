#include "raylib.h"
#include "raymath.h"

#include "headerfiles/character.c"
#include "headerfiles/charselect.c"
#include "headerfiles/tiled.c"
#include "headerfiles/menu.c"
#include "headerfiles/mapcollision.c"
#include <stdbool.h>
#include "headerfiles/battle.c"
#include <time.h>
#include <stdlib.h>
#include "headerfiles/gamemode.h"
#include "headerfiles/playerstats.c"
#include "headerfiles/bossbattle.c"
#include "json_parser.h"

#define TILE_SIZE 32
#define PLAYER_SIZE 24

// TELEPORT_LAYER_NAME and CAVE_EXIT_LAYER_NAME come from mapcollision.h
#define OVERWORLD_MAP_PATH "assets/NEWMAP.tmj"
#define OVERWORLD_TILESET "assets/map.png"
#define CAVE_MAP_PATH "assets/insideCave.tmj"
#define CAVE_TILESET "assets/map.png" // change if the cave uses a different tileset image

// Known-open landing tiles (checked against each map's collision layer)
#define CAVE_SPAWN_TILE_X 14
#define CAVE_SPAWN_TILE_Y 6
#define OVERWORLD_SPAWN_TILE_X 11
#define OVERWORLD_SPAWN_TILE_Y 7

int main()
{

    int screenWidth = 1500;
    int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Jujutsu Kaisen - raylib (C)");
    Image icon = LoadImage("Assets&resources/jjk2.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    SetTargetFPS(60);
    srand((unsigned int)time(NULL));
    const CharacterOption *chosen;
    // Load texture
    Texture2D texture = LoadTexture("Assets&resources/gojo_matched_size.png"); //Default character;
   
    LoadMenuBackgroundVideo("Assets&resources/menu_bg.mp4", screenWidth, screenHeight, 24.0f);

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

    if (!game.map)
    {
        printf("Failed to load map!\n");
        CloseWindow();
        return 1;
    }

    // Movement + collision always work in this raw, UNSCALED pixel space -

    // it matches the Tiled tile grid exactly. Only drawing gets scaled below.
    Mapport info = CalculateMapviewport(game.map->width, game.map->height, TILE_SIZE, screenWidth, screenHeight);

    // Load tileset texture
    game.tileset = LoadTexture(OVERWORLD_TILESET);

    // Find collision layer-encounterlayer-TELEPORTINTOCAVE-(DetectingtheExit-in case)
    game.collisionLayerIndex = CollisionLayer(game.map);
    game.encounterLayerIndex = EncounterLayer(game.map, ENCOUNTER_LAYER_NAME);
    game.teleportLayerIndex = EncounterLayer(game.map, TELEPORT_LAYER_NAME);
    //game.exitLayerIndex = EncounterLayer(game.map, CAVE_EXIT_LAYER_NAME);

    //MAP INFO
    printf("Map loaded: %dx%d tiles (%dx%d pixels)\n", game.map->width, game.map->height, game.map->width * TILE_SIZE, game.map->height * TILE_SIZE);
    printf("Layers: %d\n", game.map->layerCount);
    printf("Map Scale: %.2f, Offset: (%.1f, %.1f)\n", info.scale, info.offsetX, info.offsetY);
    for (int i = 0; i < game.map->layerCount; i++)
    {
        printf("  Layer %d: %s\n", i, game.map->layers[i].name);
    }
    // Tracks the player's last tile so the encounter roll only fires
    // once per NEW tile entered - not every single frame.
    int lastTileX = (int)(position.x / TILE_SIZE);
    int lastTileY = (int)(position.y / TILE_SIZE);

    //CHECKING IF IN THE TELEPORT TILE
    bool wasOnTeleportTile = false;
    bool wasOnExitTile = false;
    // "A random Beast Appeared!" popup text + fade timer
    char spawnMessage[64] = "";
    bool encounter = false;

    // InitTileRects(tileRects);
    BattleScene battle;
    BossBattleScene finalBattle;
    GameMode mode = MODE_GAME_MENU;

    PlayerStats playerstats;
    MenuState menu;
    InitMenu(&menu);
    CharSelectState charSelect; /* <-- new */

    // CharacterMove *fought;
    InitCharSelect(&charSelect);
    LoadCharSelectAssets();

    while (!WindowShouldClose())
    {

        float dt = GetFrameTime();

        Vector2 nextPos = position;
        bool moving = false;
        if (!encounter)
        {
            charactermovement(&nextPos, &currentRow, frameWidth, frameHeight, TILE_SIZE, &moving);
        }

        // character hitbox corners
        ResolveMovementCollision(game.map, game.collisionLayerIndex, &position, nextPos, PLAYER_SIZE);
        ClampPositionToMap(&position, PLAYER_SIZE, info.pixelWidth, info.pixelHeight);
        // Clamp position to screen bounds

        // Animation timing
        animation(&frameTime, &currentFrame, frameSpeed);

        // Update frame rectangle
        UpdateFrame(&frameRec, &currentFrame, &currentRow, frameWidth, frameHeight);
        int curTileX = (int)(position.x / TILE_SIZE);
        int curTileY = (int)(position.y / TILE_SIZE);

        if (mode == MODE_OVERWORLD && game.teleportLayerIndex != -1)
        {
            bool onTeleportTile = (GetTileAtMapPos(game.map, game.teleportLayerIndex, curTileX, curTileY) != 0);
            if (onTeleportTile && !wasOnTeleportTile)
            {
                info = SwitchMap(&game, CAVE_MAP_PATH, CAVE_TILESET, screenWidth, screenHeight);

                position = (Vector2){452, 202};
                lastTileX = CAVE_SPAWN_TILE_X;
                lastTileY = CAVE_SPAWN_TILE_Y;
                wasOnTeleportTile = false;
                wasOnExitTile = false;
                encounter = false;
                spawnMessage[0] = '\0';
                mode = MODE_CAVE;
                continue; // skip the rest of this frame - we just swapped maps out from under it
            }
            UpdateEncounterCheck(curTileX, curTileY, game.map, game.encounterLayerIndex, &lastTileX, &lastTileY, spawnMessage, &encounter);
            if (encounter && IsKeyPressed(KEY_SPACE))
            {
                const EnemyTemplate *randomEnemy = PickRandomEnemyTemplate();
                InitBattleScene(&battle,
                                chosen->name, chosen->fight, 4, 3,
                                randomEnemy, chosen->move1, chosen->move2, chosen->move3, chosen->move4);
                battle.player.maxHp = playerstats.maxHp;
                battle.player.currentHp = playerstats.currentHp;
                battle.player.displayedHp = playerstats.currentHp;
                mode = MODE_BATTLE;
            }

            wasOnTeleportTile = onTeleportTile;
        }
        else if (mode == MODE_CAVE)
        {
            // Falls back to a key press if you haven't added the "exit" layer to insideCave.tmj yet.
            bool onExitTile = false;
            if (game.exitLayerIndex != -1)
            {
                onExitTile = GetTileAtMapPos(game.map, game.exitLayerIndex, curTileX, curTileY) != 0;
            }
            bool exitPressed = (game.exitLayerIndex == -1) && IsKeyPressed(KEY_BACKSPACE);

            if ((onExitTile && !wasOnExitTile) || exitPressed)
            {
                info = SwitchMap(&game, OVERWORLD_MAP_PATH, OVERWORLD_TILESET, screenWidth, screenHeight);

                position = (Vector2){482, 74};
                lastTileX = OVERWORLD_SPAWN_TILE_X;
                lastTileY = OVERWORLD_SPAWN_TILE_Y;
                wasOnTeleportTile = false;
                wasOnExitTile = false;
                encounter = false;
                spawnMessage[0] = '\0';
                mode = MODE_OVERWORLD;
                continue;
            }
            UpdateEncounterBoss(curTileX, curTileY, game.map, game.encounterLayerIndex, &lastTileX, &lastTileY, spawnMessage, &encounter);
            if (encounter && IsKeyPressed(KEY_SPACE))
            {
                const BossEnemyTemplate *randomEnemy = PickRandomEnemyTemplateBOSS();
                InitBattleSceneBoss(&finalBattle,
                                chosen->name, chosen->fight, 4, 3,
                                randomEnemy, chosen->move1, chosen->move2, chosen->move3, chosen->move4);
                finalBattle.player.maxHp = playerstats.maxHp;
                finalBattle.player.currentHp = playerstats.currentHp;
                finalBattle.player.displayedHp = playerstats.currentHp;
                mode = MODE_BATTLE2;
            }
            wasOnExitTile = onExitTile;
        }

        // ---- Random encounter check (Pokemon-style tall grass) ----
        // Only rolls encounters in the overworld - remove this guard if the cave should have its own.

        else if (mode == MODE_BATTLE)
        {
            if (!IsBattleOver(&battle))
            {
                UpdateBattleScene(&battle, dt);
            }
            else
            {
                playerstats.currentHp = battle.player.currentHp;
                playerstats.maxHp = battle.player.maxHp;
                if (battle.won)
                {
                    GainExp(&playerstats, battle.rewardExp);
                }
                encounter = false;
                mode = MODE_OVERWORLD;
            }
        }
        else if (mode == MODE_BATTLE2)
        {
            if (!IsBattleOverBOSS(&finalBattle))
            {
                UpdateBattleSceneBoss(&finalBattle, dt);
            }
            else
            {
                playerstats.currentHp = finalBattle.player.currentHp;
                playerstats.maxHp = finalBattle.player.maxHp;
                if (finalBattle.won)
                {
                    GainExp(&playerstats, finalBattle.rewardExp);
                }
                encounter = false;
                mode = MODE_CAVE;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        if (mode == MODE_GAME_MENU)
        {
            UpdateDrawMenu(&menu, screenWidth, screenHeight);
            if (menu.startPressed)
            {
                mode = MODE_CHARACTER_SELECT;
            }
            if (menu.exitPressed)
            {
                break;
            }
        }
        else if (mode == MODE_CHARACTER_SELECT)
        {
            UpdateDrawCharSelect(&charSelect, screenWidth, screenHeight);

            if (charSelect.confirmed)
            {
                chosen = GetCharacterOption(charSelect.selectedIndex);
                UnloadTexture(texture);
                Texture2D texture = LoadTexture(chosen->movement);
                frameWidth = texture.width / 4;
                frameHeight = texture.height / 4;
                frameRec = (Rectangle){0, 0, frameWidth, frameHeight};

                // playerstats = (PlayerStats){0};
                InitPlayerStats(&playerstats, chosen->name);

                mode = MODE_OVERWORLD;
            }
            if (charSelect.backPressed)
            {
                mode = MODE_GAME_MENU;
            }
        }

        else if (mode == MODE_OVERWORLD || mode == MODE_CAVE)
        {

            // Character
            DrawLayer(game.map, game.collisionLayerIndex, game.encounterLayerIndex, game.teleportLayerIndex, game.exitLayerIndex, game.tileset, info.offsetX, info.offsetY, info.scale);
            DrawCharacter(texture, frameRec, position, frameWidth, frameHeight, info.scale, info.offsetX, info.offsetY);
            Drawencounter(encounter, spawnMessage, screenWidth);
            DrawDebugUI(position, nextPos, moving,game.map->width, game.map->height, info.scale,game.collisionLayerIndex);
            DrawPlayerHud(&playerstats, 1140, 20);
        }

        else if (mode == MODE_BATTLE)
        {

            DrawBattleScene(&battle);
        }
        else if (mode == MODE_BATTLE2)
        {

            DrawBattleSceneBoss(&finalBattle);
        }

        DrawFPS(10, 10);
        EndDrawing();
    }
    UnloadTexture(game.tileset);
    UnloadTileMap(game.map);
    UnloadCharSelectAssets();
    UnloadMenuBackgroundVideo();
    UnloadBattleScene(&battle);
    UnloadBattleSceneBoss(&finalBattle);
    UnloadTexture(texture);
    CloseWindow();

    return 0;
}