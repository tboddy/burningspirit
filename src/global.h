// globals

#define NORM 0.707

#define GAME_W_T 32
#define GAME_H_T 28

#define GAME_W 256
#define GAME_H 224

#define GAME_X_T 0
#define GAME_Y_T 0

#define GAME_X 0
#define GAME_Y 0

#define CLOCK_LIMIT 32000
#define COUNT_INT 8
#define INVINCIBLE_LIMIT 60 * 4

#define SFX_MENU_SELECT 64
#define SFX_MENU_CHOOSE 65
#define SFX_START_GAME 66
#define SFX_PLAYER_SHOT 67
#define SFX_ZONE_OVER 68

#define SFX_BULLET_1 69

#define SFX_EXPLOSION_1 73
#define SFX_EXPLOSION_2 74
#define SFX_EXPLOSION_3 75

#define SFX_GAME_OVER 76
#define SFX_BEAT_GAME 77

bool killBullets,
	started,
	fiveMinuteMode,
	gameOver, wonGame,
	paused, pausing;

s16 clock, timeLeft, enemyCount,
	bossHealth, bossMax;

char debugStr[8];

u32 score,
	highScore;

s16 emptyI; // lmao
void EMPTY(s16 i){emptyI = i;}

void loadGame();

void collideObstacleWithPlayer(s16);

void drawBonus(s16);

fix16 honeSpeed;
Vect2D_f16 hPos, velPos;
Vect2D_f16 hone(Vect2D_f16 pos, Vect2D_f16 target, fix16 speed, s16 lerp){
	hPos.x = target.x;
	hPos.y = target.y;
	if(lerp > 0){
		hPos.x = fix16Add(fix16Sub(hPos.x, FIX16(lerp)), FIX16(random() % (lerp * 2)));
		hPos.y = fix16Add(fix16Sub(hPos.y, FIX16(lerp)), FIX16(random() % (lerp * 2)));
	}
	honeSpeed = fix32ToFix16(getApproximatedDistance(
		fix32Sub(fix16ToFix32(pos.x), fix16ToFix32(hPos.x)),
		fix32Sub(fix16ToFix32(pos.y), fix16ToFix32(hPos.y))));
	hPos.x = fix16Mul(fix16Div(fix16Sub(hPos.x, pos.x), honeSpeed), speed);
	hPos.y = fix16Mul(fix16Div(fix16Sub(hPos.y, pos.y), honeSpeed), speed);
	return hPos;
}