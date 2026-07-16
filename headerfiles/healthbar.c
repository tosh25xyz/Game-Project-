#include "healthbar.h"
#include"battle.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void UpdateHpBarLerp(Character *c, float dt)
{
    if (c->displayedHp > c->currentHp)
    {
        c->displayedHp -= (int)(HP_LERP_SPEED * dt);
        if (c->displayedHp < c->currentHp) 
        {
            c->displayedHp = c->currentHp;
        }

    } 
    else if (c->displayedHp < c->currentHp) //ei condition dewa hoise heal er jonno jodi ami heal move add kori
    {
        c->displayedHp += (int)(HP_LERP_SPEED * dt);
        if (c->displayedHp > c->currentHp) 
        {
            c->displayedHp = c->currentHp;
        }
    }
}
void DrawHpBar(Character *c, Vector2 barPos, int barWidth, int barHeight)
{
    float pct = (float)c->displayedHp / (float)c->maxHp;
    if (pct < 0) pct = 0;

    DrawRectangle((int)barPos.x, (int)barPos.y, barWidth, barHeight, DARKGRAY);

    Color fillColor = GREEN;
    if (pct < 0.5f) fillColor = YELLOW;
    if (pct < 0.2f) fillColor = RED;

    DrawRectangle((int)barPos.x, (int)barPos.y, (int)(barWidth * pct), barHeight, fillColor);
    DrawRectangleLines((int)barPos.x, (int)barPos.y, barWidth, barHeight, BLACK);

    DrawText(c->name, (int)barPos.x, (int)barPos.y - 20, 25,WHITE);

    char hpText[16];
    snprintf(hpText, sizeof(hpText), "%d/%d", c->currentHp, c->maxHp);
    DrawText(hpText, (int)barPos.x, (int)barPos.y + barHeight + 2, 25, WHITE);
}