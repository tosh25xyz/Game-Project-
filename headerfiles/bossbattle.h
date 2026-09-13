#ifndef BOSSBATTLE_H
#define BOSSBATTLE_H

#include "raylib.h"

#define BOSS_MAX_NAME_LEN 32
#define BOSS_MAX_MOVES 5
#define BOSS_MAX_ENEMY_MOVES 4

typedef struct BossCharacter {
    char name[BOSS_MAX_NAME_LEN];
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
} BossCharacter;

typedef struct BossMove {
    char name[BOSS_MAX_NAME_LEN];
    int minDamage;
    int maxDamage;
    int usedMoves;
} BossMove;

typedef struct BossEnemyTemplate {
    char name[BOSS_MAX_NAME_LEN];
    char spritePath[128];
    int frameCols;
    int frameRows;
    int maxHp;
    BossMove moves[BOSS_MAX_ENEMY_MOVES];
    int moveCount;
} BossEnemyTemplate;

const BossEnemyTemplate *PickRandomEnemyTemplateBOSS(void);

typedef enum BossBattleState {
    BOSS_BATTLE_INTRO,
    BOSS_BATTLE_PLAYER_MENU,
    BOSS_BATTLE_PLAYER_ATTACK_IN,
    BOSS_BATTLE_PLAYER_ATTACK_HIT,
    BOSS_BATTLE_PLAYER_ATTACK_OUT,
    BOSS_BATTLE_MESSAGE_PAUSE,
    BOSS_BATTLE_ENEMY_ATTACK_IN,
    BOSS_BATTLE_ENEMY_ATTACK_HIT,
    BOSS_BATTLE_ENEMY_ATTACK_OUT,
    BOSS_BATTLE_WIN,
    BOSS_BATTLE_LOSE,
    BOSS_BATTLE_DONE          // signals to the caller (overworld) that battle is fully over
} BossBattleState;

typedef struct BossBattleScene {
    BossCharacter player;
    BossCharacter enemy;
    BossMove playerMoves[BOSS_MAX_MOVES];
    int moveCount;
    int selectedMoveIndex;   // for arrow-key menu navigation

    BossMove enemyMoves[BOSS_MAX_MOVES];
    int enemyMoveCount;

    BossMove *pendingMove;
    int pendingDamage;

    BossBattleState state;
    float stateTimer;

    char messageText[128];
    BossBattleState nextStateAfterMessage;
    // Rewards, rolled once when entering BOSS_BATTLE_WIN
    int rewardExp;
    int rewardCoins;
    Texture2D background;
    bool won;
} BossBattleScene;

void InitBattleSceneBoss(BossBattleScene *battle,
                      const char *playerName, const char *playerSpritePath,
                      int playerFrameCols, int playerFrameRows,
                      const BossEnemyTemplate *enemyTemplate,const char *move1,const char *move2,const char *move3,const char *move4);
void UnloadBattleSceneBoss(BossBattleScene *battle);

void UpdateBattleSceneBoss(BossBattleScene *battle, float dt);
void DrawBattleSceneBoss(BossBattleScene *battle);

void UpdateHpBarLerpBOSS(BossCharacter *c, float dt);
void DrawHpBarBOSS(BossCharacter *c, Vector2 barPos, int barWidth, int barHeight);
//Animation Helpers
void UpdateCharacterAnimationBOSS(BossCharacter *c, float dt);
void SetCharacterAnimBOSS(BossCharacter *c, int row, int frameCount, float frameSpeed);
int RollDamageBOSS(BossMove *move);
void ApplyDamageBOSS(BossCharacter *target, int amount);

// Returns true once the battle has fully finished (win or lose screen dismissed).
// The overworld can poll this to know when to tear down / return control to the player.
bool IsBattleOverBOSS(BossBattleScene *battle);

#endif