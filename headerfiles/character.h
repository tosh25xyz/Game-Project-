#include "raylib.h"
#include"json_parser.h"

void DrawCharacter(Texture2D texture,Rectangle frameRec,Vector2 position,int frameWidth,int frameHeight,float scale,float offsetX,float offsetY);
void charactermovement(Vector2  *position,int *currentRow,int frameWidth,int frameHeight,int tilesize,bool *moving);
void clampcharacter(Vector2  *position,int frameWidth,int frameHeight);
void animation(float *frameTime,int *currentFrame,float frameSpeed);
void UpdateFrame(Rectangle *frameRec,int *currentFrame,int *currentRow,int frameWidth,int frameHeight);
// int IsBlocked(int x, int y);
// void collisionfunc(Vector2 *nextPos,Vector2 *position,int frameWidth,int frameHeight,int tilesize);