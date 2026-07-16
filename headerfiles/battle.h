#ifndef BATTLE_H
#define BATTLE_H

#include "raylib.h"

#define MAX_NAME_LEN 32
#define MAX_MOVES 5

typedef struct Character {
    char name[MAX_NAME_LEN];
    int maxHp;
    int currentHp;
    int displayedHp;

    Texture2D sprite;
    int frameWidth;
    int frameHeight;
    Rectangle frameRec;
    int frameCount;
    int currentFrame;
    float frameTimer;
    float frameSpeed;
    int animRow;
    bool flipHorizontal;
    
    Vector2 basePos;
    Vector2 pos;
    Color tint;

    float flashTimer;
} Character;

typedef struct Move {
    char name[MAX_NAME_LEN];
    int minDamage;
    int maxDamage;
    int usedMoves;
} Move;

typedef enum BattleState {
    BATTLE_INTRO,
    BATTLE_PLAYER_MENU,
    BATTLE_PLAYER_ATTACK_IN,
    BATTLE_PLAYER_ATTACK_HIT,
    BATTLE_PLAYER_ATTACK_OUT,
    BATTLE_MESSAGE_PAUSE,
    BATTLE_ENEMY_ATTACK_IN,
    BATTLE_ENEMY_ATTACK_HIT,
    BATTLE_ENEMY_ATTACK_OUT,
    BATTLE_WIN,
    BATTLE_LOSE,
    BATTLE_DONE          // signals to the caller (overworld) that battle is fully over
} BattleState;

typedef struct BattleScene {
    Character player;
    Character enemy;
    Move playerMoves[MAX_MOVES];
    int moveCount;
    int selectedMoveIndex;   // for arrow-key menu navigation

    Move enemyMoves[MAX_MOVES];
    int enemyMoveCount;

    Move *pendingMove;
    int pendingDamage;
    
    BattleState state;
    float stateTimer;

    char messageText[128];
    BattleState nextStateAfterMessage;
    // Rewards, rolled once when entering BATTLE_WIN
    int rewardExp;
    int rewardCoins;
    Texture2D background;
    bool won;
} BattleScene;

void InitBattleScene(BattleScene *battle, const char *playerName, 
                    const char *playerSpritePath,int playerFrameCols,
                    int PlayerFrameRows,const char *enemyName,
                    const char *enemySpritePath,int enemyFrameCols,int enemyFrameRows);
void UnloadBattleScene(BattleScene *battle);

void UpdateBattleScene(BattleScene *battle, float dt);
void DrawBattleScene(BattleScene *battle);

void UpdateHpBarLerp(Character *c, float dt);
void DrawHpBar(Character *c, Vector2 barPos, int barWidth, int barHeight);
//Animation Helpers
void UpdateCharacterAnimation(Character *c, float dt);
void SetCharacterAnim(Character *c, int row, int frameCount, float frameSpeed);
int RollDamage(Move *move);
void ApplyDamage(Character *target, int amount);

// Returns true once the battle has fully finished (win or lose screen dismissed).
// The overworld can poll this to know when to tear down / return control to the player.
bool IsBattleOver(BattleScene *battle);

#endif