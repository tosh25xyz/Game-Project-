#include "raylib.h"
#include "raymath.h"
// #include"character.h"
#include "headerfiles/character.c"
#include "raygui.c"
#include "headerfiles/map.c"
// #include"map.h"
#include "headerfiles/collision.c"
// #include"raygui.h"
// const int screenWidth = 800;
#include <stdbool.h>
#include "headerfiles/enemyspawn.c"
#include "headerfiles/battle.c"
#include <time.h>
#include <stdlib.h>
#include "headerfiles/gamemode.h"
// RANDOM ENCOUNTER FUNCTION
//====================================================

int main()
{

    InitWindow(screenWidth, screenHeight, "Jujutsu Kaisen - raylib (C)");
    Image icon=LoadImage("Assets&resources/jjk2.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    SetTargetFPS(60);
    srand((unsigned int)time(NULL));
    // Load texture
    Texture2D texture = LoadTexture("Assets&resources/gojo normal walk.png");
    Texture2D tileset = LoadTexture("Assets&resources/tiles.png");

    int frameWidth = texture.width / 4;
    int frameHeight = texture.height / 4;

    Rectangle frameRec = {0, 0, frameWidth, frameHeight};
    Vector2 position = {700, 400};

    int currentFrame = 0;
    int currentRow = 0; // 0=down,1=left,2=right,3=up

    float frameTime = 0.0f;
    float frameSpeed = 0.15f;
    // Battle STATE
    bool encounter = false;

    InitTileRects(tileRects);
    BattleScene battle;
    GameMode mode;

    while (!WindowShouldClose())
    {

        

        float dt = GetFrameTime();

        if (mode == MODE_OVERWORLD)
        {
            Vector2 nextPos = position;
            bool moving = false;
            if (!encounter)
            {
                charactermovement(&nextPos, &currentRow, frameWidth, frameHeight, TILE_SIZE, &moving);
            }

            enemyspawn(&nextPos, &encounter, moving, TILE_SIZE);
            // character hitbox corners
            collisionfunc(&nextPos, &position, frameWidth, frameHeight, TILE_SIZE);

            // Clamp position to screen bounds
            clampcharacter(&position, frameWidth, frameHeight);
            // Animation timing
            animation(&frameTime, &currentFrame, frameSpeed);

            // Update frame rectangle
            UpdateFrame(&frameRec, &currentFrame, &currentRow, frameWidth, frameHeight);
            if (encounter && IsKeyPressed(KEY_SPACE))
            {
                InitBattleScene(&battle,
                    "NASIF", "Assets&resources/gojo-nasif.png", 4, 3,
                    "ZARIF", "Assets&resources/geto-nasif.png", 4, 3
                );
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
                encounter = false;
                mode = MODE_OVERWORLD;
            }
        }

       

        BeginDrawing();
        ClearBackground(BLACK);

        if (mode == MODE_OVERWORLD)
        {
            DRAWLAYERFIRST(tileset, tileRects, map, TILE_SIZE);
            DRAWLAYERsecond(tileset, tileRects, basemaps, TILE_SIZE);
            DRAWLAYERTHIRD(tileset, tileRects, maps, TILE_SIZE);
            DRAWLAYER4TH(tileset, tileRects, mapoverlap, TILE_SIZE);
            // Character
            DrawCharacter(texture, frameRec, position, frameWidth, frameHeight);
           DrawMinor(encounter, screenWidth, screenHeight);
        }
        
        else if (mode == MODE_BATTLE)
        { 
            
            DrawBattleScene(&battle);
        }
        
        DrawFPS(10, 10);
        EndDrawing();
    }
    UnloadBattleScene(&battle);
    UnloadTexture(texture);
    CloseWindow();

    return 0;
}