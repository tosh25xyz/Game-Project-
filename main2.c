/* ============================================================
   menu.c — Start Menu Screen (raylib + raygui)
   ------------------------------------------------------------
   A self-contained start screen: title, "New Game" button,
   and a difficulty dropdown (Easy / Normal / Hard).

   Build (example, MinGW):
     gcc -o test.exe test.c -Wall -std=c99 -D_DEFAULT_SOURCE ^
         -Wno-missing-braces -g -O0 ^
         -IC:/raylib/raylib/src -IC:/raylib/raylib/src/external ^
         -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm ^
         -DPLATFORM_DESKTOP

   raygui is a single-header library — put raygui.h next to
   raylib.h in your include folder. Get it from:
   https://github.com/raysan5/raygui
   ============================================================ */

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "headerfiles/raygui.h"

#include <math.h>

/* ---------------- Difficulty ---------------- */

typedef enum {
    DIFFICULTY_EASY = 0,
    DIFFICULTY_NORMAL,
    DIFFICULTY_HARD
} Difficulty;

/* Use this later wherever you spawn enemies to scale their stats */
float DifficultyEnemyHpMultiplier(Difficulty d)
{
    switch (d) {
        case DIFFICULTY_EASY:   return 0.75f;
        case DIFFICULTY_NORMAL: return 1.00f;
        case DIFFICULTY_HARD:   return 1.50f;
        default: return 1.0f;
    }
}

/* ---------------- Menu state ---------------- */

typedef struct {
    Difficulty selectedDifficulty;
    bool difficultyDropdownOpen;
    bool startPressed;
    bool exitPressed;
    float titlePulse;
} MenuState;

void InitMenu(MenuState *menu)
{
    menu->selectedDifficulty     = DIFFICULTY_NORMAL;
    menu->difficultyDropdownOpen = false;
    menu->startPressed = false;
    menu->exitPressed  = false;
    menu->titlePulse   = 0.0f;
}

/* Simple layered "night sky over ruins" backdrop, no assets needed.
   Replace with your own background texture later if you have art. */
static void DrawMenuBackdrop(int w, int h)
{
    DrawRectangleGradientV(0, 0, w, h,
        (Color){ 10, 16, 34, 255 },
        (Color){ 46, 78, 120, 255 });

    for (int i = 0; i < 6; i++) {
        int px = (i * w / 6) + 20;
        DrawRectangle(px, 0, 18, h, (Color){ 20, 30, 55, 120 });
    }

    DrawRectangleGradientV(0, (int)(h * 0.55f), w, (int)(h * 0.45f),
        (Color){ 40, 70, 110, 0 },
        (Color){ 30, 55, 90, 200 });
}

/* Call every frame. Updates menu->startPressed / exitPressed
   when the corresponding button is clicked that frame. */
void UpdateDrawMenu(MenuState *menu, int screenW, int screenH)
{
    menu->startPressed = false;
    menu->exitPressed  = false;
    menu->titlePulse  += GetFrameTime();

    BeginDrawing();

        DrawMenuBackdrop(screenW, screenH);

        /* Title */
        const char *title = "SUKUNA'S DOMAIN";   /* swap for your game's title */
        int titleSize = 46;
        int titleW = MeasureText(title, titleSize);
        DrawText(title, screenW / 2 - titleW / 2, screenH / 2 - 170, titleSize, WHITE);

        const char *subtitle = "TAP TO BEGIN";
        int subSize = 18;
        int subW = MeasureText(subtitle, subSize);
        unsigned char alpha = (unsigned char)(150 + 80 * (0.5f + 0.5f * sinf(menu->titlePulse * 2.0f)));
        DrawText(subtitle, screenW / 2 - subW / 2, screenH / 2 - 110, subSize,
                 (Color){ 200, 210, 230, alpha });

        /* Difficulty label + box position */
        Rectangle diffBox = { screenW / 2.0f - 150, screenH / 2.0f - 20, 300, 42 };
        GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
        GuiLabel((Rectangle){ diffBox.x, diffBox.y - 26, 300, 20 }, "Difficulty");

        /* Buttons — skipped while dropdown is open so clicks don't
           fall through onto them */
        Rectangle newGameBtn = { screenW / 2.0f - 150, screenH / 2.0f + 40, 300, 50 };
        Rectangle exitBtn    = { screenW / 2.0f - 150, screenH / 2.0f + 100, 300, 40 };

        if (!menu->difficultyDropdownOpen) {
            if (GuiButton(newGameBtn, "New Game")) menu->startPressed = true;
            if (GuiButton(exitBtn, "Exit"))         menu->exitPressed  = true;
        }

        /* Dropdown drawn LAST so its open list renders on top of
           everything else */
        int diffIndex = (int)menu->selectedDifficulty;
        if (GuiDropdownBox(diffBox, "Easy;Normal;Hard", &diffIndex, menu->difficultyDropdownOpen)) {
            menu->difficultyDropdownOpen = !menu->difficultyDropdownOpen;
        }
        menu->selectedDifficulty = (Difficulty)diffIndex;

    EndDrawing();
}

/* ============================================================
   Entry point — this IS your program for now. Once you're ready
   to hand off into gameplay, replace the "startPressed" branch
   below with a call into your overworld/game init instead of
   just printing to the console.
   ============================================================ */
int main(void)
{
    const int screenW = 800, screenH = 450;
    InitWindow(screenW, screenH, "JJK RPG - Start Menu");
    SetTargetFPS(60);

    MenuState menu;
    InitMenu(&menu);

    bool gameStarted = false;

    while (!WindowShouldClose()) {

        if (!gameStarted) {
            UpdateDrawMenu(&menu, screenW, screenH);

            if (menu.startPressed) {
                gameStarted = true; /* hand off to your real game here */
            }
            if (menu.exitPressed) {
                break;
            }
        } else {
            /* Placeholder for "the rest of your game" */
            BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText(TextFormat("Game started. Difficulty multiplier: %.2f",
                         DifficultyEnemyHpMultiplier(menu.selectedDifficulty)),
                         20, 20, 20, DARKGRAY);
            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}