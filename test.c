/* ============================================================
   menu.c — Main Menu / Start Screen (raylib + raygui)
   ------------------------------------------------------------
   Provides:
     - MenuState struct (holds difficulty selection + UI flags)
     - InitMenu()
     - UpdateDrawMenu()  -> call this every frame while
       currentMode == MODE_MAIN_MENU

   Integration into your existing project:
     1. Add MODE_MAIN_MENU to your GameMode enum (put it first,
        so the game boots into the menu instead of straight into
        the overworld).
     2. In your main loop's switch(currentMode), add a case that
        calls UpdateDrawMenu(&menu, &currentMode, screenW, screenH).
     3. Read menu.selectedDifficulty wherever you spawn enemies /
        compute damage, to scale stats.

   raygui is a single-header library. Only ONE .c file in your
   whole project should have:
       #define RAYGUI_IMPLEMENTATION
       #include "raygui.h"
   before including it — do that here, not in main.c, if this
   is the file you compile it in. If main.c already defines it,
   remove the #define line below and just #include "raygui.h".
   Get raygui.h from: https://github.com/raysan5/raygui
   (drop it next to raylib.h in your project's include folder)
   ============================================================ */

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

/* ---------------- Difficulty ---------------- */

typedef enum {
    DIFFICULTY_EASY = 0,
    DIFFICULTY_NORMAL,
    DIFFICULTY_HARD
} Difficulty;

static const char *DifficultyLabel(Difficulty d)
{
    
    if(d==0)
    {
        return "Easy";

    }
    else if(d==1)
    {
        return "NORMAL";
    }
    else
    {
        return  "HARD";
    }
}

/* Example: use this wherever you spawn a curse/enemy to scale it */
float DifficultyEnemyHpMultiplier(Difficulty d)
{
    
    if(d==0)
    {
        return 0.75f;

    }
    else if(d==1)
    {
        return 1.00f;
    }
    else  if(d==2)
    {
        return  1.50f;
    }
    else
    {
        return 1.00f;
    }
}

/* ---------------- GameMode (merge into your existing enum) ---------------- */

typedef enum {
    MODE_MAIN_MENU = 0,
    MODE_OVERWORLD,
    MODE_BATTLE
    /* ... your other existing modes go here ... */
} GameMode;

/* ---------------- Menu state ---------------- */

typedef struct {
    Difficulty selectedDifficulty;
    bool difficultyDropdownOpen;
    float titlePulse;      /* for the subtle "tap to begin" pulse */
} MenuState;

void InitMenu(MenuState *menu)
{
    menu->selectedDifficulty   = DIFFICULTY_NORMAL;
    menu->difficultyDropdownOpen = false;
    menu->titlePulse = 0.0f;
}

/* Draws a simple layered "night sky over ruins" backdrop without
   needing any texture assets — replace with your own background
   texture/parallax layers later if you have art for it. */
static void DrawMenuBackdrop(int w, int h)
{
    DrawRectangleGradientV(0, 0, w, h,
        (Color){ 10, 16, 34, 255 },
        (Color){ 46, 78, 120, 255 });

    /* faint "pillars" silhouettes for atmosphere */
    for (int i = 0; i < 6; i++) {
        int px = (i * w / 6) + 20;
        Color pillar = (Color){ 20, 30, 55, 120 };
        DrawRectangle(px, 0, 18, h, pillar);
    }

    /* soft cloud band near the bottom third */
    DrawRectangleGradientV(0, (int)(h * 0.55f), w, (int)(h * 0.45f),
        (Color){ 40, 70, 110, 0 },
        (Color){ 30, 55, 90, 200 });
}

/* Call every frame while in MODE_MAIN_MENU.
   Returns true the frame "New Game" was pressed. */
bool UpdateDrawMenu(MenuState *menu, GameMode *currentMode, int screenW, int screenH)
{
    bool startPressed = false;
    menu->titlePulse += GetFrameTime();

    BeginDrawing();

        DrawMenuBackdrop(screenW, screenH);

        /* Title block */
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

        /* --- Difficulty selector ---
           Drawn BEFORE the New Game button, but the dropdown list
           itself is drawn LAST (see bottom) so it renders on top. */
        Rectangle diffBox = { screenW / 2.0f - 150, screenH / 2.0f - 20, 300, 42 };
        GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
        GuiLabel((Rectangle){ diffBox.x, diffBox.y - 26, 300, 20 }, "Difficulty");

        /* --- New Game button (disabled while dropdown is open,
           so clicks don't fall through onto it) --- */
        Rectangle newGameBtn = { screenW / 2.0f - 150, screenH / 2.0f + 40, 300, 50 };
        if (!menu->difficultyDropdownOpen) {
            if (GuiButton(newGameBtn, "SHURU")) {
                startPressed = true;
                *currentMode = MODE_OVERWORLD; /* hand off to your game */
            }
        }

        Rectangle exitBtn = { screenW / 2.0f - 150, screenH / 2.0f + 100, 300, 40 };
        if (!menu->difficultyDropdownOpen) {
            if (GuiButton(exitBtn, "SHESH")) {
                /* set a global "shouldClose" flag instead of calling
                   CloseWindow() directly, so your main loop can exit cleanly */
            }
        }

        /* Dropdown drawn last -> renders its open list on top of everything */
        int diffIndex = (int)menu->selectedDifficulty;
        if (GuiDropdownBox(diffBox, "EASY;NORMAL;HARD", &diffIndex, menu->difficultyDropdownOpen)) {
            menu->difficultyDropdownOpen = !menu->difficultyDropdownOpen;
        }
        menu->selectedDifficulty = (Difficulty)diffIndex;

    EndDrawing();

    return startPressed;
}

/* ============================================================
   Minimal standalone demo — remove this main() when you fold
   the module into your existing project (which already has
   its own main() / game loop).
   ============================================================ */
int main(void)
{
    const int screenW = 800, screenH = 450;
    InitWindow(screenW, screenH, "JJK RPG - Main Menu Demo");
    SetTargetFPS(60);

    MenuState menu;
    InitMenu(&menu);
    GameMode currentMode = MODE_MAIN_MENU;

    while (!WindowShouldClose()) {
        switch (currentMode) {
            case MODE_MAIN_MENU:
                UpdateDrawMenu(&menu, &currentMode, screenW, screenH);
                break;
            case MODE_OVERWORLD:
                BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawText(TextFormat("Overworld start. Difficulty: %s",
                             DifficultyLabel(menu.selectedDifficulty)), 20, 20, 20, DARKGRAY);
                EndDrawing();
                break;
            default: break;
        }
    }

    CloseWindow();
    return 0;
}
