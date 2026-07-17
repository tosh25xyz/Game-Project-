#include"raylib.h"
#include<stdio.h>
#include"collision.h"


void collisionfunc(Vector2 *nextPos,Vector2 *position,int frameWidth,int frameHeight,int tilesize)
{
    float scale = 3.0f;
    float playerWidth = frameWidth / scale;
    float playerHeight = frameHeight / scale;
    int leftTile   = nextPos->x / (tilesize);
    int rightTile  = (nextPos->x + playerWidth - 1) / (tilesize);
    int topTile    = nextPos->y / (tilesize);
    int bottomTile = (nextPos->y + playerHeight - 1) / (tilesize);
    if (!IsBlocked(leftTile, topTile) &&
    !IsBlocked(rightTile, topTile) &&
    !IsBlocked(leftTile, bottomTile) &&
    !IsBlocked(rightTile, bottomTile))
{
    *position = *nextPos;
}



}
int IsBlocked(int x, int y)
{
    // boundary check
    // if (x < 0 || y < 0 || x >= 30 || y >= 20)
    //     return 1;

    if(mapoverlap[y][x] == 11 || mapoverlap[y][x] == 19 || mapoverlap[y][x] == 17|| mapoverlap[y][x] == 16)
    {
        return 1;
    }
    else if(mapoverlap[y][x] == 13 || mapoverlap[y][x] == 29 || mapoverlap[y][x] == 30)
    {
        return 1;
    }
    else if(mapoverlap[y][x] == 25 || mapoverlap[y][x] == 28 || mapoverlap[y][x] == 26)
    {
        return 1;
    }
    else if(mapoverlap[y][x] == 23 || mapoverlap[y][x] == 24 || mapoverlap[y][x] == 22 || mapoverlap[y][x]==14 ||mapoverlap[y][x]==12)
    {
        return 1;
    }
    else if(mapoverlap[y][x] == 10 || mapoverlap[y][x] == 8 || mapoverlap[y][x] == 7 || mapoverlap[y][x]==6 ||mapoverlap[y][x]==5 || mapoverlap[y][x]==35) 
    {
        return 1;
    }


    return 0;
}