#ifndef CHARSELECT_H
#define CHARSELECT_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    const char *name;
    const char *spritePath;   /* preview image shown on the select screen */
    const char *tagline;      /* short flavor line under the name */
    const char *movement;
    const char *fight;
    const char *move1;
    const char *move2;
    const char *move3;
    const char *move4;
    // const CharacterMove *main;
    
} CharacterOption;

// typedef struct{
//      const char *move1;
//      const char *move2;
//     const char *move3;
//      const char *move4;
//      const char *move5;
// } CharacterMove;
typedef struct {
    int  selectedIndex;
    bool confirmed;      /* true only on the frame "Select" is clicked */
    bool backPressed;    /* true only on the frame "Back" is clicked   */
} CharSelectState;

void InitCharSelect(CharSelectState *cs);
void LoadCharSelectAssets(void);
void UnloadCharSelectAssets(void);
void UpdateDrawCharSelect(CharSelectState *cs, int screenW, int screenH);

int GetCharacterCount(void);
const CharacterOption *GetCharacterOption(int index);

#endif