#include"menu.h"
#include<stdio.h>
//#include"gamemode.h"
//#include"../raygui.h"

#define RAYGUI_IMPLEMENTATION
 #include "raygui.h"


void InitMenu(MenuState *menu)
{
    menu->selectedDifficulty     = DIFFICULTY_NORMAL;
    menu->difficultyDropdownOpen = false;
    menu->startPressed = false;
    menu->exitPressed  = false;
    menu->titlePulse   = 0.0f;
}
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
// static const char *DifficultyLabel(Difficulty d)
// {
    
//     if(d==0)
//     {
//         return "Easy";

//     }
//     else if(d==1)
//     {
//         return "NORMAL";
//     }
//     else
//     {
//         return  "HARD";
//     }
// }
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
void UpdateDrawMenu(MenuState *menu, int screenW, int screenH)
{
    menu->startPressed = false;
    menu->exitPressed  = false;
    menu->titlePulse  += GetFrameTime();

    

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

    
}