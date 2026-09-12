#include "raylib.h"
#include "raymath.h"

// #define RAYGUI_IMPLEMENTATION
// #include "raygui.h"
// #include"character.h"
#include "headerfiles/character.c"
#include "headerfiles/charselect.c"

#include "headerfiles/tiled.c"
#include "headerfiles/menu.c"
// #include"map.h"
#include "headerfiles/mapcollision.c"
// #include"raygui.h"
// const int screenWidth = 800;
#include <stdbool.h>
//#include "headerfiles/enemyspawn.c"
#include "headerfiles/battle.c"
#include <time.h>
#include <stdlib.h>
#include "headerfiles/gamemode.h"
#include "headerfiles/playerstats.c"
#include "json_parser.h"
// RANDOM ENCOUNTER FUNCTION
//====================================================
#define TILE_SIZE 32
#define PLAYER_SIZE 24
int main()
{

    int screenWidth=1500;
    int screenHeight=900;
    InitWindow(screenWidth, screenHeight, "Jujutsu Kaisen - raylib (C)");
    Image icon = LoadImage("Assets&resources/jjk2.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    SetTargetFPS(60);
    srand((unsigned int)time(NULL));
    const CharacterOption *chosen;
    // Load texture
    Texture2D texture = LoadTexture("Assets&resources/gojo_matched_size.png");
    // texture = LoadTexture("Assets&resources/player.png");
    //Texture2D tileset = LoadTexture("Assets&resources/tiles.png");
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
    // Tracks the player's last tile so the encounter roll only fires
    // once per NEW tile entered - not every single frame.
    int lastTileX = (int)(position.x / TILE_SIZE);
    int lastTileY = (int)(position.y / TILE_SIZE);

    // "A random Beast Appeared!" popup text + fade timer
    char spawnMessage[64] = "";
    bool encounter = false;

    //InitTileRects(tileRects);
    BattleScene battle;
    GameMode mode = MODE_GAME_MENU;

    PlayerStats playerstats;
    MenuState menu;
    InitMenu(&menu);
    CharSelectState charSelect; /* <-- new */
    
    // CharacterMove *fought;
    InitCharSelect(&charSelect);
    LoadCharSelectAssets();
    // Difficulty d;
    //  int playerMaxHp = 100;
    //  int playerCurrentHp = 100;
    //InitPlayerStats(&playerstats, "NASIF");
    while (!WindowShouldClose())
    {

        float dt = GetFrameTime();
        // if(mode ==MODE_GAME_MENU)
        // {
        //     DifficultyEnemyHpMultiplier(d);
        // }

        if (mode == MODE_OVERWORLD)
        {
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
        // ---- Random encounter check (Pokemon-style tall grass) ----
            UpdateEncounterCheck(curTileX,curTileY, game.map, game.encounterLayerIndex,&lastTileX, &lastTileY, spawnMessage, &encounter);
            //int tileUnderPlayer = GetTileAtMapPos(game.map, game.collisionLayerIndex, curTileX, curTileY);
            if (encounter && IsKeyPressed(KEY_SPACE))
            {
                const EnemyTemplate *randomEnemy=PickRandomEnemyTemplate();
                InitBattleScene(&battle,
                                chosen->name, chosen->fight, 4, 3,
                                randomEnemy,chosen->move1,chosen->move2,chosen->move3,chosen->move4);
                battle.player.maxHp = playerstats.maxHp;
                battle.player.currentHp = playerstats.currentHp;
                battle.player.displayedHp = playerstats.currentHp;
                mode = MODE_BATTLE;
            }
        }
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

        else if (mode == MODE_OVERWORLD)
        {
            
            // Character
            DrawLayer(game.map,game.collisionLayerIndex,game.encounterLayerIndex,game.tileset, info.offsetX, info.offsetY, info.scale);
            DrawCharacter(texture, frameRec, position, frameWidth, frameHeight, info.scale, info.offsetX, info.offsetY);
            Drawencounter(encounter,spawnMessage,screenWidth);
            //DrawDebugUI(position, nextPos, moving,game.map->width, game.map->height, info.scale,game.collisionLayerIndex, tileUnderPlayer);
            DrawPlayerHud(&playerstats, 1140, 20);
        }

        else if (mode == MODE_BATTLE)
        {

            DrawBattleScene(&battle);
        }

        DrawFPS(10, 10);
        EndDrawing();
    }
    UnloadTexture(game.tileset);
    UnloadTileMap(game.map);
    UnloadCharSelectAssets();
    UnloadMenuBackgroundVideo();
    UnloadBattleScene(&battle);
    UnloadTexture(texture);
    CloseWindow();

    return 0;
}