#ifndef PLAYERSTATS_H
#define PLAYERSTATS_H

#include "raylib.h"

typedef struct PlayerStats {
    char name[32];
    int level;
    int currentHp;
    int maxHp;
    int currentExp;
    int expToNextLevel;
} PlayerStats;

void InitPlayerStats(PlayerStats *stats, const char *name);
void GainExp(PlayerStats *stats, int amount);
void DrawPlayerHud(PlayerStats *stats, int x, int y);

#endif