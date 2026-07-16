#include "battle.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HP_LERP_SPEED     60.0f
#define FLASH_DURATION    0.15f
#define MESSAGE_PAUSE_DURATION 1.2f
#define ATTACK_MOVE_SPEED 900.0f
#define ATTACK_HIT_PAUSE  0.2f
#define ATTACK_LUNGE_DIST 100.0f

//ANIMATION CONVERSION
#define ANIME_ROW_IDLE 0
#define ANIME_ROW_ATTACK 1
#define ANIME_ROW_HURT 2

#define ATTACK_FRAME_COUNT 4
#define ATTACK_FRAME_SPEED 0.08f
#define IDLE_FRAME_SPEED 0.30f

static float Vec2Distance(Vector2 a, Vector2 b)
{
    float dx = a.x - b.x, dy = a.y - b.y;
   // float dx =900 - 400, dy = 900 - 300;
    return sqrtf(dx * dx + dy * dy);
}

static Vector2 MoveToward(Vector2 current, Vector2 target, float maxDist)
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

void SetCharacterAnim(Character *c, int row, int frameCount, float frameSpeed)
{
    c->animRow = row;
    c->frameCount = frameCount;
    c->frameSpeed = frameSpeed;
    c->currentFrame = 0;
    c->frameTimer = 0.0f;
    c->frameRec.x = 0;
    c->frameRec.y = (float)(row * c->frameHeight);
}
void UpdateCharacterAnimation(Character *c, float dt)
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
static void InitCharacterSprite(Character *c, const char *spritePath, int frameCols, int frameRows, bool flip)
{
    c->sprite = LoadTexture(spritePath);
    SetTextureFilter(c->sprite, TEXTURE_FILTER_POINT);
    c->frameWidth = c->sprite.width / frameCols;
    c->frameHeight = c->sprite.height / frameRows;
    c->flipHorizontal = flip;
    c->frameRec = (Rectangle){ 0, 0, (float)c->frameWidth, (float)c->frameHeight };
    SetCharacterAnim(c, ANIME_ROW_IDLE, frameCols, IDLE_FRAME_SPEED);
}

void InitBattleScene(BattleScene *battle,
                      const char *playerName, const char *playerSpritePath,
                      int playerFrameCols, int playerFrameRows,
                      const char *enemyName, const char *enemySpritePath,
                      int enemyFrameCols, int enemyFrameRows)
{
    strncpy(battle->player.name, playerName, MAX_NAME_LEN - 1);
    battle->player.name[MAX_NAME_LEN - 1] = '\0';
    battle->player.maxHp = 100;
    battle->player.currentHp = 100;
    battle->player.displayedHp = 100;
    
    battle->player.basePos = (Vector2){270, 500 };
    battle->player.pos = battle->player.basePos;
    battle->player.tint = WHITE;
    battle->player.flashTimer = 0.0f;
    InitCharacterSprite(&battle->player, playerSpritePath, playerFrameCols, playerFrameRows, false);

    strncpy(battle->enemy.name, enemyName, MAX_NAME_LEN - 1);
    battle->enemy.name[MAX_NAME_LEN - 1] = '\0';
    battle->enemy.maxHp = 100;
    battle->enemy.currentHp = 100;
    battle->enemy.displayedHp = 100;
   
    battle->enemy.basePos = (Vector2){ 1050, 510 };
    battle->enemy.pos = battle->enemy.basePos;
    battle->enemy.tint = WHITE;
    battle->enemy.flashTimer = 0.0f;
    InitCharacterSprite(&battle->enemy, enemySpritePath, enemyFrameCols, enemyFrameRows, true);
    strcpy(battle->playerMoves[0].name, "Nasif's 50 note Slash");
    battle->playerMoves[0].minDamage = 5;
    battle->playerMoves[0].maxDamage = 15;
    battle->playerMoves[0].usedMoves=2;

    strcpy(battle->playerMoves[1].name, "MOOD SWING STRIKE");
    battle->playerMoves[1].minDamage = 16;
    battle->playerMoves[1].maxDamage = 17;
    battle->playerMoves[1].usedMoves=2;
    strcpy(battle->playerMoves[2].name, "BUSTY  RED  HEAD FLASH");
    battle->playerMoves[2].minDamage = 20;
    battle->playerMoves[2].maxDamage = 30;
    battle->playerMoves[2].usedMoves=2;
    strcpy(battle->playerMoves[3].name, "NASIF POKE");
    battle->playerMoves[3].minDamage = 3;
    battle->playerMoves[3].maxDamage = 6;
    battle->playerMoves[3].usedMoves=2;
    strcpy(battle->playerMoves[4].name, " DOMAIN EXPANSION: RAGEBAIT ");
    battle->playerMoves[4].minDamage = 25;
    battle->playerMoves[4].maxDamage = 60;
    battle->playerMoves[4].usedMoves=-1;
    battle->moveCount = 5;
    battle->selectedMoveIndex = 0;

    strcpy(battle->enemyMoves[0].name, " MINOR Bite ");
    battle->enemyMoves[0].minDamage = 8;
    battle->enemyMoves[0].maxDamage = 9;

    strcpy(battle->enemyMoves[1].name, " MINOR TERRITORY ");
    battle->enemyMoves[1].minDamage = 20;
    battle->enemyMoves[1].maxDamage = 40;

    strcpy(battle->enemyMoves[2].name, "MINOR Growl Slam");
    battle->enemyMoves[2].minDamage = 14;
    battle->enemyMoves[2].maxDamage = 25;

    strcpy(battle->enemyMoves[3].name, "ONLY MINOR SLASH");
    battle->enemyMoves[3].minDamage = 16;
    battle->enemyMoves[3].maxDamage = 17;

    battle->enemyMoveCount = 4;

    battle->pendingMove = NULL;
    battle->pendingDamage = 0;

    battle->state = BATTLE_INTRO;
    battle->stateTimer = 0.0f;
    strcpy(battle->messageText, "");
    battle->nextStateAfterMessage = BATTLE_PLAYER_MENU;

    battle->rewardExp = 0;
    battle->rewardCoins = 0;
    battle->won=false;
    battle->background = LoadTexture("Assets&resources/battle2.png");
}

void UnloadBattleScene(BattleScene *battle)
{
    UnloadTexture(battle->player.sprite);
    UnloadTexture(battle->enemy.sprite);
    UnloadTexture(battle->background);
}

int RollDamage(Move *move)
{
    return move->minDamage + rand() % (move->maxDamage - move->minDamage + 1);
}

void ApplyDamage(Character *target, int amount)
{
    target->currentHp -= amount;
    if (target->currentHp < 0) target->currentHp = 0;
    target->flashTimer = FLASH_DURATION;
}

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

static void UpdateFlash(Character *c, float dt)
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

static Move *PickEnemyMove(BattleScene *battle)
{
    int idx = rand() % battle->enemyMoveCount;
    return &battle->enemyMoves[idx];
}

bool IsBattleOver(BattleScene *battle)
{
    return battle->state == BATTLE_DONE;
}

void UpdateBattleScene(BattleScene *battle, float dt)
{
    battle->stateTimer += dt;

    UpdateHpBarLerp(&battle->player, dt);
    UpdateHpBarLerp(&battle->enemy, dt);
    UpdateFlash(&battle->player, dt);
    UpdateFlash(&battle->enemy, dt);
    UpdateCharacterAnimation(&battle->player, dt);
    UpdateCharacterAnimation(&battle->enemy, dt);

    if (battle->state == BATTLE_INTRO) {
        if (battle->stateTimer > 1.0f) {
            battle->state = BATTLE_PLAYER_MENU;
            battle->stateTimer = 0.0f;
        }
    }
    else if (battle->state == BATTLE_PLAYER_MENU) {
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
                battle->state = BATTLE_PLAYER_ATTACK_IN;
                battle->stateTimer = 0.0f;
            }
            
           
            
        }
        
    }
    else if (battle->state == BATTLE_PLAYER_ATTACK_IN) {
        if (battle->stateTimer == dt) {
                SetCharacterAnim(&battle->player, ANIME_ROW_ATTACK, ATTACK_FRAME_COUNT, ATTACK_FRAME_SPEED);
            }
        Vector2 target = {
            battle->enemy.basePos.x - ATTACK_LUNGE_DIST,
            battle->enemy.basePos.y
        };
        battle->player.pos = MoveToward(battle->player.pos, target, ATTACK_MOVE_SPEED * dt);
        if (Vec2Distance(battle->player.pos, target) < 1.0f) {
            battle->state = BATTLE_PLAYER_ATTACK_HIT;
            battle->stateTimer = 0.0f;
        }
    }
    else if (battle->state == BATTLE_PLAYER_ATTACK_HIT) {
        if (battle->stateTimer == dt) {
            int dmg = RollDamage(battle->pendingMove);
            battle->pendingDamage = dmg;
            ApplyDamage(&battle->enemy, dmg);
            SetCharacterAnim(&battle->enemy, ANIME_ROW_HURT, 1, 1.0f);
            snprintf(battle->messageText, sizeof(battle->messageText),
                     "%s used %s!\n%s took %d damage!",
                     battle->player.name, battle->pendingMove->name,
                     battle->enemy.name, dmg);
        }
        if (battle->stateTimer > ATTACK_HIT_PAUSE) {
            battle->state = BATTLE_PLAYER_ATTACK_OUT;
            battle->stateTimer = 0.0f;
        }
    }
    else if (battle->state == BATTLE_PLAYER_ATTACK_OUT) {
        battle->player.pos = MoveToward(battle->player.pos, battle->player.basePos, ATTACK_MOVE_SPEED * dt);
        if (Vec2Distance(battle->player.pos, battle->player.basePos) < 1.0f) {
            battle->player.pos = battle->player.basePos;
            SetCharacterAnim(&battle->player, ANIME_ROW_IDLE, 4, IDLE_FRAME_SPEED);
            SetCharacterAnim(&battle->enemy, ANIME_ROW_IDLE, 4, IDLE_FRAME_SPEED);
            battle->state = BATTLE_MESSAGE_PAUSE;
            battle->stateTimer = 0.0f;
            if(battle->enemy.currentHp<=0)
            {
                battle->nextStateAfterMessage= BATTLE_WIN;
                battle->won=true;
            }
            else
            {
                battle->nextStateAfterMessage=BATTLE_ENEMY_ATTACK_IN;
            }
            // battle->nextStateAfterMessage =
            //     (battle->enemy.currentHp <= 0) ? BATTLE_WIN : BATTLE_ENEMY_ATTACK_IN;
        }
    }
    else if (battle->state == BATTLE_MESSAGE_PAUSE) {
        if (battle->stateTimer > MESSAGE_PAUSE_DURATION) {
            battle->state = battle->nextStateAfterMessage;
            battle->stateTimer = 0.0f;

            if (battle->state == BATTLE_ENEMY_ATTACK_IN) {
                battle->pendingMove = PickEnemyMove(battle);
            }
            if (battle->state == BATTLE_WIN) {
                battle->rewardExp = 60 + rand() % 21;
                battle->rewardCoins = 100 + rand() % 41;
            }
        }
    }
    else if (battle->state == BATTLE_ENEMY_ATTACK_IN) {
        if (battle->stateTimer == dt) {
                SetCharacterAnim(&battle->enemy, ANIME_ROW_ATTACK, ATTACK_FRAME_COUNT, ATTACK_FRAME_SPEED);
            }
        Vector2 target = {
            battle->player.basePos.x + ATTACK_LUNGE_DIST,
            battle->player.basePos.y
        };
        battle->enemy.pos = MoveToward(battle->enemy.pos, target, ATTACK_MOVE_SPEED * dt);
        if (Vec2Distance(battle->enemy.pos, target) < 1.0f) {
            battle->state = BATTLE_ENEMY_ATTACK_HIT;
            battle->stateTimer = 0.0f;
        }
    }
    else if (battle->state == BATTLE_ENEMY_ATTACK_HIT) {
        if (battle->stateTimer == dt) {
            int dmg = RollDamage(battle->pendingMove);
            battle->pendingDamage = dmg;
            ApplyDamage(&battle->player, dmg);
            SetCharacterAnim(&battle->player, ANIME_ROW_HURT, 1, 1.0f);
            snprintf(battle->messageText, sizeof(battle->messageText),
                     "%s used %s!\n%s took %d damage!",
                     battle->enemy.name, battle->pendingMove->name,
                     battle->player.name, dmg);
        }
        if (battle->stateTimer > ATTACK_HIT_PAUSE) {
            battle->state = BATTLE_ENEMY_ATTACK_OUT;
            battle->stateTimer = 0.0f;
        }
    }
    else if (battle->state == BATTLE_ENEMY_ATTACK_OUT) {
        battle->enemy.pos = MoveToward(battle->enemy.pos, battle->enemy.basePos, ATTACK_MOVE_SPEED * dt);
        if (Vec2Distance(battle->enemy.pos, battle->enemy.basePos) < 1.0f) {
            battle->enemy.pos = battle->enemy.basePos;
            SetCharacterAnim(&battle->enemy, ANIME_ROW_IDLE, 4, IDLE_FRAME_SPEED);
            SetCharacterAnim(&battle->player, ANIME_ROW_IDLE, 4, IDLE_FRAME_SPEED);
            battle->state = BATTLE_MESSAGE_PAUSE;
            battle->stateTimer = 0.0f;
            if(battle->player.currentHp<=0)
            {
                battle->nextStateAfterMessage= BATTLE_LOSE;
            }
            else
            {
                battle->nextStateAfterMessage=BATTLE_PLAYER_MENU;
            }
            // battle->nextStateAfterMessage =
            //     (battle->player.currentHp <= 0) ? BATTLE_LOSE : BATTLE_PLAYER_MENU;
        }
    }
    else if (battle->state == BATTLE_WIN) {
        if (battle->stateTimer > 0.5f &&
            (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))) {
            battle->state = BATTLE_DONE;
        }
    }
    else if (battle->state == BATTLE_LOSE) {
        if (battle->stateTimer > 0.5f &&
            (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))) {
            battle->state = BATTLE_DONE;
        }
    }
    // else if (battle->state == BATTLE_DONE) {
    //     // কিছুই করার নেই
    // }
}
void DrawBattleScene(BattleScene *battle)
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
    DrawHpBar(&battle->player, (Vector2){ 60, 60 }, 300, 30);
    DrawHpBar(&battle->enemy, (Vector2){ 1140, 60 }, 300, 30);

    if (battle->state == BATTLE_PLAYER_MENU) {
        DrawRectangle(0, 700, 2000 , 700, (Color){ 20, 20, 20, 220 });
        DrawText("Fight", 20, 710, 32, WHITE);
        for (int i = 0; i < battle->moveCount; i++) {
            char line[64];
            snprintf(line, sizeof(line), "%d  %s", i + 1, battle->playerMoves[i].name);
            Color col;
            // if(i==4)
            // {
            //     if(battle->enemy.currentHp<=50)
            //     {
            //         col=WHITE;
            //     }
            //     else
            //     {
            //         col=RED;
            //     }
            // }
            // else if(battle->playerMoves[i].usedMoves==0)
            // {
            //     col=RED;
            // }

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
            // Color col = (i == battle->selectedMoveIndex) ? YELLOW : WHITE;
            // little arrow marker on the selected move, like a real menu cursor
            if (i == battle->selectedMoveIndex) {
                DrawText(">", 5, 760 + i * 22, 28, YELLOW);
            }
            DrawText(line, 20, 760 + i * 22, 28, col);
        }
        DrawText("Arrows + Enter, or press 1-4", 500, 800, 24, GRAY);
    }

    bool showMessageBox =
        battle->state == BATTLE_MESSAGE_PAUSE ||
        battle->state == BATTLE_PLAYER_ATTACK_HIT ||
        battle->state == BATTLE_PLAYER_ATTACK_OUT ||
        battle->state == BATTLE_ENEMY_ATTACK_HIT ||
        battle->state == BATTLE_ENEMY_ATTACK_OUT;

    if (showMessageBox) {
        DrawRectangle(0, 700, 2000, 700, (Color){ 20, 20, 20, 220 });
        DrawText(battle->messageText, 20, 750, 30, WHITE);
    }

    if (battle->state == BATTLE_WIN) {
        DrawRectangle(0, 700, 2000, 700, (Color){ 20, 20, 20, 220 });
        DrawText("YOU WIN!", 20, 710, 42, GOLD);

        char rewardLine[64];
        snprintf(rewardLine, sizeof(rewardLine), "EXP +%d", battle->rewardExp);
        DrawText(rewardLine, 20, 750, 30, WHITE);

        snprintf(rewardLine, sizeof(rewardLine), "Coins +%d", battle->rewardCoins);
        DrawText(rewardLine, 20, 790, 30, WHITE);

        DrawText("Press Enter to continue", 20, 850, 16, GRAY);
    }

    if (battle->state == BATTLE_LOSE) {
        DrawRectangle(0, 700, 2000, 700, (Color){ 20, 20, 20, 220 });
        DrawText("YOU LOST...", 20, 710, 32, RED);
        DrawText("Press Enter to continue", 20, 850, 16, GRAY);
    }
}