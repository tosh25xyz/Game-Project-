#include "bossbattle.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BOSS_HP_LERP_SPEED     60.0f
#define BOSS_FLASH_DURATION    0.15f
#define BOSS_MESSAGE_PAUSE_DURATION 1.2f
#define BOSS_ATTACK_MOVE_SPEED 900.0f
#define BOSS_ATTACK_HIT_PAUSE  0.2f
#define BOSS_ATTACK_LUNGE_DIST 100.0f

//ANIMATION CONVERSION
#define BOSS_ANIME_ROW_IDLE 0
#define BOSS_ANIME_ROW_ATTACK 1
#define BOSS_ANIME_ROW_HURT 2

#define BOSS_ATTACK_FRAME_COUNT 4
#define BOSS_ATTACK_FRAME_SPEED 0.08f
#define BOSS_IDLE_FRAME_SPEED 0.30f

static float Vec2DistanceBOSS(Vector2 a, Vector2 b)
{
    float dx = a.x - b.x, dy = a.y - b.y;
   // float dx =900 - 400, dy = 900 - 300;
    return sqrtf(dx * dx + dy * dy);
}

static Vector2 MoveTowardBOSS(Vector2 current, Vector2 target, float maxDist)
{
    float dx = target.x - current.x;
    float dy = target.y - current.y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist <= maxDist || dist == 0.0f)
    {
        return target;
    } 
    float t = maxDist / dist;
    return (Vector2){ current.x + dx * t, current.y + dy * t };
}

void SetCharacterAnimBOSS(BossCharacter *c, int row, int frameCount, float frameSpeed)
{
    c->animRow = row;
    c->frameCount = frameCount;
    c->frameSpeed = frameSpeed;
    c->currentFrame = 0;
    c->frameTimer = 0.0f;
    c->frameRec.x = 0;
    c->frameRec.y = (float)(row * c->frameHeight);
}
void UpdateCharacterAnimationBOSS(BossCharacter *c, float dt)
{
    c->frameTimer += dt;
    if (c->frameTimer >= c->frameSpeed) {
        c->frameTimer = 0.0f;
        c->currentFrame = (c->currentFrame + 1) % c->frameCount;
        c->frameRec.x = (float)(c->currentFrame * c->frameWidth);
        c->frameRec.y = (float)(c->animRow * c->frameHeight);
    }
    // negative width mirrors the sprite horizontally when drawn
    if(c->flipHorizontal)
    {
        c->frameRec.width=-(float)c->frameWidth;
    }
    else
    {
        c->frameRec.width=(float)c->frameWidth;
    }
    
}
static void InitCharacterSpriteBOSS(BossCharacter *c, const char *spritePath, int frameCols, int frameRows, bool flip)
{
    c->sprite = LoadTexture(spritePath);
    SetTextureFilter(c->sprite, TEXTURE_FILTER_POINT);
    c->frameWidth = c->sprite.width / frameCols;
    c->frameHeight = c->sprite.height / frameRows;
    c->flipHorizontal = flip;
    c->frameRec = (Rectangle){ 0, 0, (float)c->frameWidth, (float)c->frameHeight };
    SetCharacterAnimBOSS(c, BOSS_ANIME_ROW_IDLE, frameCols, BOSS_IDLE_FRAME_SPEED);
}
static const BossEnemyTemplate enemyPoolBOSS[] = {
    {
        "DAEMON", "Assets&resources/muzan.png", 4, 3, 100,
        {
            { " DARK CRY ", 18, 29, -1 },
            { " DARK DIMENSION", 20, 40, -1 },
            { "SOUL CRUSHER", 14, 25, -1 },
            { "18'S WIN", 16, 17, -1 },
        },
        4
    },
    
};

#define BOSS_ENEMY_POOL_SIZE (sizeof(enemyPoolBOSS) / sizeof(enemyPoolBOSS[0]))

const BossEnemyTemplate *PickRandomEnemyTemplateBOSS(void)
{
    int idx = rand() % BOSS_ENEMY_POOL_SIZE;
    return &enemyPoolBOSS[idx];
}



void InitBattleSceneBoss(BossBattleScene *battle,const char *playerName, const char *playerSpritePath,int playerFrameCols, int playerFrameRows,const BossEnemyTemplate *enemyTemplate,const char *move1,const char *move2,const char *move3,const char *move4)
{
    strncpy(battle->player.name, playerName, BOSS_MAX_NAME_LEN - 1);
    battle->player.name[BOSS_MAX_NAME_LEN - 1] = '\0';
    battle->player.maxHp = 100;
    battle->player.currentHp = 100;
    battle->player.displayedHp = 100;
    
    battle->player.basePos = (Vector2){270, 340 };
    battle->player.pos = battle->player.basePos;
    battle->player.tint = WHITE;
    battle->player.flashTimer = 0.0f;
    InitCharacterSpriteBOSS(&battle->player, playerSpritePath, playerFrameCols, playerFrameRows, false);

    strncpy(battle->enemy.name, enemyTemplate->name , BOSS_MAX_NAME_LEN - 1);
    battle->enemy.name[BOSS_MAX_NAME_LEN - 1] = '\0';
    battle->enemy.maxHp = 100;
    battle->enemy.currentHp = 100;
    battle->enemy.displayedHp = 100;
   
    battle->enemy.basePos = (Vector2){ 1050, 350 };
    battle->enemy.pos = battle->enemy.basePos;
    battle->enemy.tint = WHITE;
    battle->enemy.flashTimer = 0.0f;
    InitCharacterSpriteBOSS(&battle->enemy, enemyTemplate->spritePath, enemyTemplate->frameCols, enemyTemplate->frameRows, true);
    strcpy(battle->playerMoves[0].name, move1);
    battle->playerMoves[0].minDamage = 5;
    battle->playerMoves[0].maxDamage = 15;
    battle->playerMoves[0].usedMoves=2;

    strcpy(battle->playerMoves[1].name, move2);
    battle->playerMoves[1].minDamage = 16;
    battle->playerMoves[1].maxDamage = 17;
    battle->playerMoves[1].usedMoves=2;
    strcpy(battle->playerMoves[2].name, move3);
    battle->playerMoves[2].minDamage = 20;
    battle->playerMoves[2].maxDamage = 30;
    battle->playerMoves[2].usedMoves=2;
    strcpy(battle->playerMoves[3].name, move4);
    battle->playerMoves[3].minDamage = 3;
    battle->playerMoves[3].maxDamage = 6;
    battle->playerMoves[3].usedMoves=2;
    strcpy(battle->playerMoves[4].name, "DOMAIN:RAGEBAIT NASIF");
    battle->playerMoves[4].minDamage = 25;
    battle->playerMoves[4].maxDamage = 60;
    battle->playerMoves[4].usedMoves=-1;
    battle->moveCount = 5;
    battle->selectedMoveIndex = 0;

    for(int i=0;i<enemyTemplate->moveCount;i++)
    {
        battle->enemyMoves[i]=enemyTemplate->moves[i];

    }
    battle->enemyMoveCount=enemyTemplate->moveCount;

    battle->pendingMove = NULL;
    battle->pendingDamage = 0;

    battle->state = BOSS_BATTLE_INTRO;
    battle->stateTimer = 0.0f;
    strcpy(battle->messageText, "");
    battle->nextStateAfterMessage = BOSS_BATTLE_PLAYER_MENU;

    battle->rewardExp = 0;
    battle->rewardCoins = 0;
    battle->won=false;
    battle->background = LoadTexture("Assets&resources/battlecave.png");
}

void UnloadBattleSceneBoss(BossBattleScene *battle)
{
    UnloadTexture(battle->player.sprite);
    UnloadTexture(battle->enemy.sprite);
    UnloadTexture(battle->background);
}

int RollDamageBOSS(BossMove *move)
{
    return move->minDamage + rand() % (move->maxDamage - move->minDamage + 1);
}

void ApplyDamageBOSS(BossCharacter *target, int amount)
{
    target->currentHp -= amount;
    if (target->currentHp < 0) target->currentHp = 0;
    target->flashTimer = BOSS_FLASH_DURATION;
}

void UpdateHpBarLerpBOSS(BossCharacter *c, float dt)
{
    if (c->displayedHp > c->currentHp)
    {
        c->displayedHp -= (int)(BOSS_HP_LERP_SPEED * dt);
        if (c->displayedHp < c->currentHp) 
        {
            c->displayedHp = c->currentHp;
        }

    } 
    else if (c->displayedHp < c->currentHp) //ei condition dewa hoise heal er jonno jodi ami heal move add kori
    {
        c->displayedHp += (int)(BOSS_HP_LERP_SPEED * dt);
        if (c->displayedHp > c->currentHp) 
        {
            c->displayedHp = c->currentHp;
        }
    }
}

static void UpdateFlashBOSS(BossCharacter *c, float dt)
{
    if (c->flashTimer > 0.0f) 
    {
        c->flashTimer -= dt;
        c->tint = RED;
        if (c->flashTimer <= 0.0f) 
        {
            c->flashTimer = 0.0f;
            c->tint = WHITE;
        }
    }
}

void DrawHpBarBOSS(BossCharacter *c, Vector2 barPos, int barWidth, int barHeight)
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

static BossMove *PickEnemyMoveBOSS(BossBattleScene *battle)
{
    int idx = rand() % battle->enemyMoveCount;
    return &battle->enemyMoves[idx];
}

bool IsBattleOverBOSS(BossBattleScene *battle)
{
    return battle->state == BOSS_BATTLE_DONE;
}

void UpdateBattleSceneBoss(BossBattleScene *battle, float dt)
{
    battle->stateTimer += dt;

    UpdateHpBarLerpBOSS(&battle->player, dt);
    UpdateHpBarLerpBOSS(&battle->enemy, dt);
    UpdateFlashBOSS(&battle->player, dt);
    UpdateFlashBOSS(&battle->enemy, dt);
    UpdateCharacterAnimationBOSS(&battle->player, dt);
    UpdateCharacterAnimationBOSS(&battle->enemy, dt);

    if (battle->state == BOSS_BATTLE_INTRO) {
        if (battle->stateTimer > 1.0f) {
            battle->state = BOSS_BATTLE_PLAYER_MENU;
            battle->stateTimer = 0.0f;
        }
    }
    else if (battle->state == BOSS_BATTLE_PLAYER_MENU) {
        if (IsKeyPressed(KEY_DOWN)) {
            battle->selectedMoveIndex = (battle->selectedMoveIndex + 1) % battle->moveCount;
        }
        if (IsKeyPressed(KEY_UP)) {
            battle->selectedMoveIndex =
                (battle->selectedMoveIndex - 1 + battle->moveCount) % battle->moveCount;
        }
            
        int chosen = -1;
        if (IsKeyPressed(KEY_ONE)) chosen = 0;
        if (IsKeyPressed(KEY_TWO)) chosen = 1;
        if (IsKeyPressed(KEY_THREE)) chosen = 2;
        if (IsKeyPressed(KEY_FOUR)) chosen = 3;
        if (IsKeyPressed(KEY_ENTER)) 
        {
            chosen = battle->selectedMoveIndex;
            
        }

        if (chosen >= 0 && chosen < battle->moveCount   ) {
            bool canUSE=false;
            if(chosen!=4)
            {
                canUSE=(battle->playerMoves[chosen].usedMoves !=0);
            }
            else if (chosen==4 && battle->enemy.currentHp<=50)
            {
                canUSE=true;
                
            }
            if(canUSE)
            {
                if(battle->playerMoves[chosen].usedMoves>0)
                {
                    battle->playerMoves[chosen].usedMoves--;
                }
                battle->pendingMove = &battle->playerMoves[chosen];
                battle->state = BOSS_BATTLE_PLAYER_ATTACK_IN;
                battle->stateTimer = 0.0f;
            }
            
           
            
        }
        
    }
    else if (battle->state == BOSS_BATTLE_PLAYER_ATTACK_IN) {
        if (battle->stateTimer == dt) {
                SetCharacterAnimBOSS(&battle->player, BOSS_ANIME_ROW_ATTACK, BOSS_ATTACK_FRAME_COUNT, BOSS_ATTACK_FRAME_SPEED);
            }
        Vector2 target = {
            battle->enemy.basePos.x - BOSS_ATTACK_LUNGE_DIST,
            battle->enemy.basePos.y
        };
        battle->player.pos = MoveTowardBOSS(battle->player.pos, target, BOSS_ATTACK_MOVE_SPEED * dt);
        if (Vec2DistanceBOSS(battle->player.pos, target) < 1.0f) {
            battle->state = BOSS_BATTLE_PLAYER_ATTACK_HIT;
            battle->stateTimer = 0.0f;
        }
    }
    else if (battle->state == BOSS_BATTLE_PLAYER_ATTACK_HIT) {
        if (battle->stateTimer == dt) {
            int dmg = RollDamageBOSS(battle->pendingMove);
            battle->pendingDamage = dmg;
            ApplyDamageBOSS(&battle->enemy, dmg);
            SetCharacterAnimBOSS(&battle->enemy, BOSS_ANIME_ROW_HURT, 1, 1.0f);
            snprintf(battle->messageText, sizeof(battle->messageText),
                     "%s used %s!\n%s took %d damage!",
                     battle->player.name, battle->pendingMove->name,
                     battle->enemy.name, dmg);
        }
        if (battle->stateTimer > BOSS_ATTACK_HIT_PAUSE) {
            battle->state = BOSS_BATTLE_PLAYER_ATTACK_OUT;
            battle->stateTimer = 0.0f;
        }
    }
    else if (battle->state == BOSS_BATTLE_PLAYER_ATTACK_OUT) {
        battle->player.pos = MoveTowardBOSS(battle->player.pos, battle->player.basePos, BOSS_ATTACK_MOVE_SPEED * dt);
        if (Vec2DistanceBOSS(battle->player.pos, battle->player.basePos) < 1.0f) {
            battle->player.pos = battle->player.basePos;
            SetCharacterAnimBOSS(&battle->player, BOSS_ANIME_ROW_IDLE, 4, BOSS_IDLE_FRAME_SPEED);
            SetCharacterAnimBOSS(&battle->enemy, BOSS_ANIME_ROW_IDLE, 4, BOSS_IDLE_FRAME_SPEED);
            battle->state = BOSS_BATTLE_MESSAGE_PAUSE;
            battle->stateTimer = 0.0f;
            if(battle->enemy.currentHp<=0)
            {
                battle->nextStateAfterMessage= BOSS_BATTLE_WIN;
                battle->won=true;
            }
            else
            {
                battle->nextStateAfterMessage=BOSS_BATTLE_ENEMY_ATTACK_IN;
            }
        }
    }
    else if (battle->state == BOSS_BATTLE_MESSAGE_PAUSE) {
        if (battle->stateTimer > BOSS_MESSAGE_PAUSE_DURATION) {
            battle->state = battle->nextStateAfterMessage;
            battle->stateTimer = 0.0f;

            if (battle->state == BOSS_BATTLE_ENEMY_ATTACK_IN) {
                battle->pendingMove = PickEnemyMoveBOSS(battle);
            }
            if (battle->state == BOSS_BATTLE_WIN) {
                battle->rewardExp = 60 + rand() % 21;
                battle->rewardCoins = 100 + rand() % 41;
            }
        }
    }
    else if (battle->state == BOSS_BATTLE_ENEMY_ATTACK_IN) {
        if (battle->stateTimer == dt) {
                SetCharacterAnimBOSS(&battle->enemy, BOSS_ANIME_ROW_ATTACK, BOSS_ATTACK_FRAME_COUNT, BOSS_ATTACK_FRAME_SPEED);
            }
        Vector2 target = {
            battle->player.basePos.x + BOSS_ATTACK_LUNGE_DIST,
            battle->player.basePos.y
        };
        battle->enemy.pos = MoveTowardBOSS(battle->enemy.pos, target, BOSS_ATTACK_MOVE_SPEED * dt);
        if (Vec2DistanceBOSS(battle->enemy.pos, target) < 1.0f) {
            battle->state = BOSS_BATTLE_ENEMY_ATTACK_HIT;
            battle->stateTimer = 0.0f;
        }
    }
    else if (battle->state == BOSS_BATTLE_ENEMY_ATTACK_HIT) {
        if (battle->stateTimer == dt) {
            int dmg = RollDamageBOSS(battle->pendingMove);
            battle->pendingDamage = dmg;
            ApplyDamageBOSS(&battle->player, dmg);
            SetCharacterAnimBOSS(&battle->player, BOSS_ANIME_ROW_HURT, 1, 1.0f);
            snprintf(battle->messageText, sizeof(battle->messageText),
                     "%s used %s!\n%s took %d damage!",
                     battle->enemy.name, battle->pendingMove->name,
                     battle->player.name, dmg);
        }
        if (battle->stateTimer > BOSS_ATTACK_HIT_PAUSE) {
            battle->state = BOSS_BATTLE_ENEMY_ATTACK_OUT;
            battle->stateTimer = 0.0f;
        }
    }
    else if (battle->state == BOSS_BATTLE_ENEMY_ATTACK_OUT) {
        battle->enemy.pos = MoveTowardBOSS(battle->enemy.pos, battle->enemy.basePos, BOSS_ATTACK_MOVE_SPEED * dt);
        if (Vec2DistanceBOSS(battle->enemy.pos, battle->enemy.basePos) < 1.0f) {
            battle->enemy.pos = battle->enemy.basePos;
            SetCharacterAnimBOSS(&battle->enemy, BOSS_ANIME_ROW_IDLE, 4, BOSS_IDLE_FRAME_SPEED);
            SetCharacterAnimBOSS(&battle->player, BOSS_ANIME_ROW_IDLE, 4, BOSS_IDLE_FRAME_SPEED);
            battle->state = BOSS_BATTLE_MESSAGE_PAUSE;
            battle->stateTimer = 0.0f;
            if(battle->player.currentHp<=0)
            {
                battle->nextStateAfterMessage= BOSS_BATTLE_LOSE;
            }
            else
            {
                battle->nextStateAfterMessage=BOSS_BATTLE_PLAYER_MENU;
            }
        }
    }
    else if (battle->state == BOSS_BATTLE_WIN) {
        if (battle->stateTimer > 0.5f &&
            (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))) {
            battle->state = BOSS_BATTLE_DONE;
        }
    }
    else if (battle->state == BOSS_BATTLE_LOSE) {
        if (battle->stateTimer > 0.5f &&
            (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))) {
            battle->state = BOSS_BATTLE_DONE;
        }
    }
    // else if (battle->state == BOSS_BATTLE_DONE) {
    //     // কিছুই করার নেই
    // }
}
void DrawBattleSceneBoss(BossBattleScene *battle)
{
    ClearBackground(RAYWHITE);
     DrawTexturePro(
        battle->background,
        (Rectangle){ 0, 0, (float)battle->background.width, (float)battle->background.height },
        (Rectangle){ 0, 0, 1500, 900 },   // screen-এর যত বড়/height, ততটাই stretch হবে
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );

   
    //DrawLine(0, 700, 2000, 700, BLUE);
    float scale =2.0f;
    Rectangle playerDest = {
    battle->player.pos.x,
    battle->player.pos.y,
    battle->player.frameWidth/ scale,
    battle->player.frameHeight / scale
};
Vector2 origin={0,0};
    DrawTexturePro(battle->player.sprite, battle->player.frameRec,playerDest, origin,0.0f ,battle->player.tint);
    Rectangle enemyDest = {
    battle->enemy.pos.x,
    battle->enemy.pos.y,
    battle->enemy.frameWidth / scale,
    battle->enemy.frameHeight / scale
};
    DrawTexturePro(battle->enemy.sprite, battle->enemy.frameRec, enemyDest,origin,0.0f, battle->enemy.tint);
    DrawHpBarBOSS(&battle->player, (Vector2){ 60, 60 }, 300, 30);
    DrawHpBarBOSS(&battle->enemy, (Vector2){ 1140, 60 }, 300, 30);

    if (battle->state == BOSS_BATTLE_PLAYER_MENU) {
        DrawRectangle(0, 700, 2000 , 700, (Color){ 20, 20, 20, 220 });
        DrawText("Fight", 20, 710, 32, WHITE);
        for (int i = 0; i < battle->moveCount; i++) {
            char line[64];
            snprintf(line, sizeof(line), "%d  %s", i + 1, battle->playerMoves[i].name);
            Color col;

            if(i==battle->selectedMoveIndex)
            {
                if(i == 4 && battle->enemy.currentHp<=50)
                {
                    col=YELLOW;
                }
                else if(battle->playerMoves[i].usedMoves==0)
                {
                    col=RED;
                }
                else if(i==4)
                {
                    col=RED;
                }
                else
                {
                    col=YELLOW;
                }

            }
            else
            {
                 col= WHITE;
            }
            // little arrow marker on the selected move, like a real menu cursor
            if (i == battle->selectedMoveIndex) {
                DrawText(">", 5, 760 + i * 22, 28, YELLOW);
            }
            DrawText(line, 20, 760 + i * 22, 28, col);
        }
        DrawText("Arrows + Enter, or press 1-4", 500, 800, 24, GRAY);
    }

    bool showMessageBox =
        battle->state == BOSS_BATTLE_MESSAGE_PAUSE ||
        battle->state == BOSS_BATTLE_PLAYER_ATTACK_HIT ||
        battle->state == BOSS_BATTLE_PLAYER_ATTACK_OUT ||
        battle->state == BOSS_BATTLE_ENEMY_ATTACK_HIT ||
        battle->state == BOSS_BATTLE_ENEMY_ATTACK_OUT;

    if (showMessageBox) {
        DrawRectangle(0, 700, 2000, 700, (Color){ 20, 20, 20, 220 });
        DrawText(battle->messageText, 20, 750, 30, WHITE);
    }

    if (battle->state == BOSS_BATTLE_WIN) {
        DrawRectangle(0, 700, 2000, 700, (Color){ 20, 20, 20, 220 });
        DrawText("YOU WIN!", 20, 710, 42, GOLD);

        char rewardLine[64];
        snprintf(rewardLine, sizeof(rewardLine), "EXP +%d", battle->rewardExp);
        DrawText(rewardLine, 20, 750, 30, WHITE);

        snprintf(rewardLine, sizeof(rewardLine), "Coins +%d", battle->rewardCoins);
        DrawText(rewardLine, 20, 790, 30, WHITE);

        DrawText("Press Enter to continue", 20, 850, 16, GRAY);
    }

    if (battle->state == BOSS_BATTLE_LOSE) {
        DrawRectangle(0, 700, 2000, 700, (Color){ 20, 20, 20, 220 });
        DrawText("YOU LOST...", 20, 710, 32, RED);
        DrawText("Press Enter to continue", 20, 850, 16, GRAY);
    }
}