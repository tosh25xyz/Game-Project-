#ifndef HEALTH_H
#define HEALTH_H
#include "raylib.h"
#include"battle.h"
void UpdateHpBarLerp(Character *c, float dt);
void DrawHpBar(Character *c, Vector2 barPos, int barWidth, int barHeight);
#endif