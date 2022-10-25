// main

#include <genesis.h>
#include <resources.h>

#include "global.h"
#include "structs.h"
#include "controls.h"
#include "start.h"
#include "background.h"
#include "explosions.h"
#include "bullets.h"
#include "powerups.h"
#include "player.h"
#include "enemies.h"
#include "stage.h"
#include "chrome.h"


// game loop

static void loadInternals(){
	VDP_setScreenWidth256();
	JOY_init();
	JOY_setEventHandler(&updateControls);
	SPR_init(127, 0, 0);
	VDP_loadFont(font.tileset, DMA);
	VDP_setPalette(PAL1, flandre.palette -> data);
	VDP_setPalette(PAL2, bg2.palette -> data);
	VDP_setTextPalette(1);

	XGM_setPCM(SFX_START_GAME, sfxStartGame, sizeof(sfxStartGame));
	XGM_setPCM(SFX_PLAYER_SHOT, sfxPlayerShot, sizeof(sfxPlayerShot));
	XGM_setPCM(SFX_BULLET_1, sfxBullet, sizeof(sfxBullet));
	XGM_setPCM(SFX_EXPLOSION_1, sfxExplosion1, sizeof(sfxExplosion1));
	XGM_setPCM(SFX_EXPLOSION_2, sfxExplosion2, sizeof(sfxExplosion2));
	XGM_setPCM(SFX_GAME_OVER, sfxGameOver, sizeof(sfxGameOver));
	XGM_setPCM(SFX_BEAT_GAME, sfxBeatGame, sizeof(sfxBeatGame));
}

void loadGame(){
	started = TRUE;
	timeLeft = 120;
	loadBg();
	loadPlayer();
	loadChrome();
	loadStage();
	XGM_startPlay(&bgmLevel);
}

static void updateGame(){
	if(!paused){
		updatePlayer();
		updateEnemies();
		updatePowerups();
		updateStage();
		updateBullets();
		updateBg();
		updateChrome();
		updateExplosions();
	}
	if(ctrl.start && !pausing && !gameOver){
		paused = paused ? FALSE : TRUE;
		pausing = TRUE;
		paused ? XGM_pausePlay() : XGM_resumePlay();
	} else if(!ctrl.start && pausing) pausing = FALSE;
	if(!paused){
		clock++;
		if(clock >= CLOCK_LIMIT) clock -= CLOCK_LIMIT;
	}
}

// main loop

int main(){
	loadInternals();
	// loadGame();
	loadStart();
	while(1){
		started ? updateGame() : updateStart();
		SPR_update();
		SYS_doVBlankProcess();
	}
	return(0);
}