#include "playerstats.h"
#include <string.h>
#include <stdio.h>
#define BASE_EXP_TO_LEVEL 100
#define baseexp 0;
#define HP_GAIN_PER_LEVEL 10

void InitPlayerStats(PlayerStats *stats,const char *name)
{
    strncpy(stats->name,name,sizeof(stats->name)-1);
    stats->name[sizeof(stats->name)-1]='\0';
    stats->level=1;
    stats->maxHp=100;
    stats->currentHp=100;
    stats->currentExp=0;
    stats->expToNextLevel=BASE_EXP_TO_LEVEL;

}
void GainExp(PlayerStats *stats,int amount)
{
    stats->currentExp+=amount;
    //WE use while loop cuz Level Might Increase Based on EXP eARNED 
    while(stats->currentExp>=stats->expToNextLevel)
    {
        stats->currentExp -=stats->expToNextLevel;
        stats->level++;
        stats->maxHp+=HP_GAIN_PER_LEVEL;
        stats->currentHp=stats->maxHp;
        stats->expToNextLevel=stats->level*baseexp;
    }
}
void DrawPlayerHud(PlayerStats *stats,int x,int y)
{
    char namelevel[64];
    snprintf(namelevel,sizeof(namelevel),"%s Lv:%d",stats->name,stats->level);
    DrawText(namelevel,x,y,20,BLACK);
    int BarWidth=220, BarHeight=18;
    //--------HP-BAR-----------------
    float hpPct=(float)stats->currentHp/(float)stats->maxHp;
    if(hpPct<0){
        hpPct=0;
    }
    DrawRectangle(x,y+24,BarWidth,BarHeight,BLACK);
    Color hpColor=GREEN;
    if(hpPct<0.5f)
    {
        hpColor=YELLOW;
    }

    if(hpPct<0.2f)
    {
        hpColor=RED;
    }
    DrawRectangle(x,y+24,(int)(BarWidth*hpPct),BarHeight,hpColor);
    DrawRectangleLines(x,y+24,BarWidth,BarHeight,BLACK);


    //-----EXP-BAR-----------------
    float expPct=(float)stats->currentHp/(float)stats->expToNextLevel;
    if(expPct<0)
    {
        expPct=0;
    }
    DrawRectangle(x,y+46,BarWidth,10,DARKGRAY);
    DrawRectangle(x,y+46,(int)(BarWidth*expPct),10,SKYBLUE);
    DrawRectangleLines(x,y+46,BarWidth,10,BLACK);
    char exptext[32];
    snprintf(exptext,sizeof(exptext),"EXP %d/%d",stats->currentExp,stats->expToNextLevel);
    DrawText(exptext,x,y+58,12,BLACK);

}