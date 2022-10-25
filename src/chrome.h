// chrome

#define CHROME_I 64

// score

#define SCORE_X 1
#define SCORE_Y 1
#define SCORE_LENGTH 8

u32 lastScore;
char scoreStr[SCORE_LENGTH];

static void loadScore(){

	VDP_drawText("00000000", SCORE_X, SCORE_Y);
}

static void updateScore(){
	if(lastScore != score){
		uintToStr(score, scoreStr, SCORE_LENGTH);
		VDP_drawText(scoreStr, SCORE_X, SCORE_Y);
		lastScore = score;
	}
}


// time

#define TIME_Y 26
#define TIME_X 27

s16 timeClock, minLeft, secLeft;
char minStr[1];
char secStr[2];

bool timeOver;

static void updateTime(){
	if(timeClock > 0) timeClock--;
	else if(timeLeft >= 0){
		timeLeft--;
		minLeft = timeLeft >= 60 ? 1 : 0;
		secLeft = timeLeft >= 60 ? timeLeft - 60 : timeLeft;
		intToStr(minLeft, minStr, 1);
		VDP_drawText(minStr, TIME_X, TIME_Y);
		VDP_drawText(":", TIME_X + 1, TIME_Y);
		intToStr(secLeft, secStr, 2);
		VDP_drawText(secStr, TIME_X + 2, TIME_Y);
		timeClock = 60;
	} else if(timeLeft <= 0){
		VDP_drawText("0:00", TIME_X, TIME_Y);
		gameOver = TRUE;
		timeOver = TRUE;
	}
}


// boss

#define BOSS_TILE_X 1
#define BOSS_TILE_Y 2
#define BOSS_TILE_COUNT 30
#define BOSS_TILE_PX 30 * 8

s16 lastBossHealth;
fix16 bossLimit;

static void updateBoss(){
	if(lastBossHealth != bossHealth){
		bossLimit = fix16Div(fix16Mul(fix16Div(FIX16(bossHealth), FIX16(bossMax)), BOSS_TILE_PX), 8);
		for(s16 x = 0; x < BOSS_TILE_COUNT; x++){
			if(bossHealth <= 0 || FIX16(x) >= bossLimit){
				VDP_clearText(x + BOSS_TILE_X, BOSS_TILE_Y, 1);
			}
		}
		for(s16 x = 0; x < BOSS_TILE_COUNT; x++){
			if(bossHealth > 1 && FIX16(x) < bossLimit) VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, 0, 0, CHROME_I), x + BOSS_TILE_X, BOSS_TILE_Y);
		}
		lastBossHealth = bossHealth;
	}
}


// lives

#define LIVES_X 30
#define LIVES_Y 1

s16 lastLives;
static void updateLives(){
	if(player.lives <= 0 && lastLives != 0){
		lastLives = 0;
		VDP_clearTextArea(LIVES_X - 2, LIVES_Y, 3, 1);
	} else if(lastLives != player.lives){
		VDP_clearTextArea(LIVES_X - 2, LIVES_Y, 3, 1);
		lastLives = player.lives;
		for(s16 i = 0; i < player.lives; i++){
			VDP_drawText("#", LIVES_X - i, LIVES_Y);
		}
	}
}


// bonus

#define LABEL_X 13
#define LABEL_Y 13
#define LABEL_W 8

s16 labelClock;
char bonusStr[4];

void drawBonus(s16 bonus){
	intToStr(bonus, bonusStr, 4);
	VDP_drawText("BONUS!", LABEL_X, LABEL_Y);
	VDP_drawText(bonusStr, LABEL_X + 1, LABEL_Y + 1);
	labelClock = 180;
}

void drawLevelStart(){
	VDP_drawText("2 MINUTE", LABEL_X - 1, LABEL_Y);
	VDP_drawText("START!", LABEL_X, LABEL_Y + 1);
	labelClock = 180;
}

static void updateLabel(){
	if(labelClock > 0){
		labelClock--;
		if(labelClock <= 0){
			labelClock = 0;
			VDP_clearTextArea(LABEL_X - 1, LABEL_Y, LABEL_W, 2);
		}
	}
}


// game over

bool drewGameOver;
static void drawGameOver(){
	drewGameOver = TRUE;
	uintToStr(score, scoreStr, SCORE_LENGTH);
	VDP_drawText(wonGame ? "YOU WIN!" : (timeOver ? "TIMEOVER" : "YOU LOSE"), LABEL_X - 1, LABEL_Y - 1);
	VDP_drawText("YOUR SCORE", LABEL_X - 2, LABEL_Y + 1);
	VDP_drawText(scoreStr, LABEL_X - 1, LABEL_Y + 2);
	if(!wonGame) XGM_stopPlay();
	XGM_startPlayPCM(wonGame ? SFX_BEAT_GAME : SFX_GAME_OVER, 0, SOUND_PCM_CH1);
}


// loop

void loadChrome(){
	VDP_loadTileSet(bossBar.tileset, CHROME_I, DMA);
	loadScore();
	drawLevelStart();
}

void updateChrome(){
	updateBoss();
	updateLabel();
	if(gameOver){
		if(!drewGameOver) drawGameOver();
	} else {
		updateTime();
		updateScore();
		updateLives();
	}
	// VDP_drawText(debugStr, 0, 27);
}