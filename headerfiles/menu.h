#include "raylib.h"


//#include"gamemode.h"
/* ---------------- Difficulty ---------------- */

typedef enum {
    DIFFICULTY_EASY = 0,
    DIFFICULTY_NORMAL,
    DIFFICULTY_HARD
} Difficulty;

typedef struct {
    Difficulty selectedDifficulty;
    bool difficultyDropdownOpen;
    bool startPressed;
    bool exitPressed;
    float titlePulse;
} MenuState;

void InitMenu(MenuState *menu);
static void DrawMenuBackdrop(int w, int h);
float DifficultyEnemyHpMultiplier(Difficulty d);
void UpdateDrawMenu(MenuState *menu, int screenW, int screenH);
void LoadMenuBackgroundVideo(const char *videoPath, int width, int height, float fps);
void UnloadMenuBackgroundVideo(void);