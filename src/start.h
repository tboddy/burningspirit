// start

bool selectingStartMenu;

#define START_W 32
#define START_H 28
#define START_I 32

#define TRANS_TIME 240

s16 startClock;

static void updateTransition(s16 startTime, bool last){
	if(startClock >= startTime && startClock < startTime + TRANS_TIME){
		switch(startClock - startTime){
			case 0: VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, START_I), 0, 0, START_W, START_H); break;
			case 5: VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, START_I + 1), 0, 0, START_W, START_H); break;
			case 10: VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, START_I + 2), 0, 0, START_W, START_H); break;
			case 15: VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, START_I + 3), 0, 0, START_W, START_H); break;
			case 20: VDP_clearTileMapRect(BG_A, 0, 0, START_W, START_H); break;
			case 200: if(!last) VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, START_I + 3), 0, 0, START_W, START_H); break;
			case 205: if(!last) VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, START_I + 2), 0, 0, START_W, START_H); break;
			case 210: if(!last) VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, START_I + 1), 0, 0, START_W, START_H); break;
			case 215: if(!last) VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, START_I), 0, 0, START_W, START_H); break;
		}
	}
}


// sega

#define SEGA_X 9
#define SEGA_Y 12

static void drawStartSega(){
	// VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, START_I), 0, 0, START_W, START_H);
	VDP_drawImageEx(BG_B, &startBoddy, TILE_ATTR_FULL(PAL1, 0, 0, 0, START_I + 8), SEGA_X, SEGA_Y, 0, DMA);
}


// intro

#define INTRO_X 2
#define INTRO_Y 10
static void drawStartIntro(){
	VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, START_I), 0, 0, START_W, START_H);
	VDP_drawImageEx(BG_B, &startIntro, TILE_ATTR_FULL(PAL1, 0, 0, 0, START_I + 32), INTRO_X, INTRO_Y, 0, DMA);
}


// events

static void loadGameFromStart(){
	XGM_stopPlay();
	XGM_startPlayPCM(SFX_START_GAME, 1, SOUND_PCM_CH2);
	selectingStartMenu = FALSE;
	VDP_clearTileMapRect(BG_A, 0, 0, START_W, START_H);
	VDP_clearTileMapRect(BG_B, 0, 0, START_W, START_H);
	loadGame();
}


// bg logo

#define LOGO_X 4
#define LOGO_Y 6


static void drawStartLogo(){
	VDP_clearTileMapRect(BG_B, 0, 0, START_W, START_H);
	VDP_drawImageEx(BG_B, &startLogo1, TILE_ATTR_FULL(PAL1, 0, 0, 0, START_I + 16), LOGO_X, LOGO_Y, 0, DMA);
	XGM_startPlay(&bgmStart);
}


// menu

#define MENU_X 7
#define MENU_Y 17

static void drawStartMenu(){
	VDP_drawText("PRESS ANY BUTTON", MENU_X + 1, MENU_Y);
	VDP_drawText("FOR 2 MINUTE GAME!", MENU_X, MENU_Y + 2);
	VDP_drawText("2022 T.BODDY", 1, 26);
	VDP_drawText("VER 0.1", 24, 26);
}

static void selectStartMenu(){
	// selectingStartMenu = TRUE;
	// fiveMinuteMode = !fiveMinuteMode;
	// VDP_drawText(fiveMinuteMode ? "5" : "2", MENU_X + 2, MENU_Y + 2);
}


// loop

void loadStart(){
	VDP_loadTileSet(startFade1.tileset, START_I, DMA);
	VDP_loadTileSet(startFade2.tileset, START_I + 1, DMA);
	VDP_loadTileSet(startFade3.tileset, START_I + 2, DMA);
	VDP_loadTileSet(startFade4.tileset, START_I + 3, DMA);
	drawStartSega();
	// loadGameFromStart();
}

bool didIntro;

void updateStart(){
	updateTransition(0, FALSE);
	updateTransition(TRANS_TIME, FALSE);
	if(startClock == TRANS_TIME) drawStartIntro();
	else if(startClock >= TRANS_TIME * 2) {
		if(startClock == TRANS_TIME * 2) drawStartLogo();
		else if(startClock == TRANS_TIME * 2 + 25){
			drawStartMenu();
			didIntro = TRUE;
		}
		updateTransition(TRANS_TIME * 2, TRUE);
		if((ctrl.start || ctrl.a || ctrl.b || ctrl.c) && !selectingStartMenu) loadGameFromStart();
	}
	if(!didIntro) startClock++;
}