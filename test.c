
#include "raylib.h"



// #include "headerfiles/charselect.c"

#include "headerfiles/map.c"


// #include"raygui.h"
 const int screenWidth = 1500;
const int screenHeight = 800;


int main()
{
 InitWindow(screenWidth, screenHeight, "Jujutsu Kaisen - raylib (C)");
    
    SetTargetFPS(60);
    //srand((unsigned int)time(NULL));

    // texture = LoadTexture("Assets&resources/player.png");
    Texture2D tileset = LoadTexture("Assets&resources/map.png");

    InitTileRects(tileRects);
while(!WindowShouldClose())

{
     BeginDrawing();
        ClearBackground(BLACK);
            DRAWLAYERFIRST(tileset, tileRects, map, TILE_SIZE);
            // DRAWLAYERsecond(tileset, tileRects, basemaps, TILE_SIZE);
            // DRAWLAYERTHIRD(tileset, tileRects, maps, TILE_SIZE);
            // DRAWLAYER4TH(tileset, tileRects, mapoverlap, TILE_SIZE);

     DrawFPS(10, 10);
        EndDrawing();
    
}
    
    CloseWindow();
return 0;
}