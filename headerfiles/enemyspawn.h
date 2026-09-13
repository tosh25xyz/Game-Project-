#include"raylib.h"
#include<stdbool.h>
#include"map.h"
void enemyspawn(Vector2 *nextPos,bool *encounter,bool moving,int tilesize);
bool CheckPokemonEncounter(int currentTile, bool isMoving);
void DrawMinor(bool encounter,int screenWidth,int screenHeight);