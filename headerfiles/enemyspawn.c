#include "raylib.h"
#include <stdbool.h>
#include "enemyspawn.h"
void DrawMinor(bool encounter,int screenWidth,int screenHeight)
{
if (encounter)
        {
            DrawRectangle(
                0,
                0,
                screenWidth,
                screenHeight,
                Fade(BLACK, 0.7f)
            );

            DrawText(
                "MINOR HUNTER ZARIF Appeared!",
                500,
                250,
                40,
                WHITE
            );

            DrawText(
                "Press ENTER to continue",
                500,
                320,
                20,
                YELLOW
            );
        }
}
void enemyspawn(Vector2 *nextPos,bool *encounter,bool moving,int tilesize)
{
    int tileX = nextPos->x / tilesize;
        int tileY = nextPos->y /tilesize;

     if (tileX >= 0 && tileX < 30 &&
            tileY >= 0 && tileY < 20)
        {
            //------------------------------------------------
            // CHECK RANDOM ENCOUNTER
            //------------------------------------------------

            if (CheckPokemonEncounter(mapoverlap[tileY][tileX],
                                      moving))
            {
                *encounter = true;
            }
        }

        //ENCOUNTER RESET
    // if (*encounter && IsKeyPressed(KEY_SPACE))
    //     {
    //         *encounter = false;
    //     }
}
bool CheckPokemonEncounter(int currentTile, bool isMoving)
{
    // Grass tile
    if (currentTile == 0 && isMoving)
    {
        // Random encounter chance
        int randomChance = GetRandomValue(0, 700);

        // Encounter trigger
        if (randomChance == 6)
        {
            return true;
        }
    }

    return false;
}
