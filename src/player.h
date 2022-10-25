// player

#define PLAYER_OFF_X FIX16(16)
#define PLAYER_OFF_Y FIX16(20)
#define PLAYER_INIT_X FIX16(40)
#define PLAYER_INIT_Y FIX16(GAME_H / 2)

#define HITBOX_OFF FIX16(4)

#define SHOT_INTERVAL 15


// spawn

static void spawnPlayer(){
	player.pos.x = PLAYER_INIT_X;
	player.pos.y = PLAYER_INIT_Y;
	player.shotClock = SHOT_INTERVAL;
	player.image = SPR_addSprite(&suika,
		fix16ToInt(fix16Sub(player.pos.x, PLAYER_OFF_X)),
		fix16ToInt(fix16Sub(player.pos.y, PLAYER_OFF_Y)),
		TILE_ATTR(PAL1, 0, 0, 0));
	player.hitboxImage = SPR_addSprite(&hitbox,
		fix16ToInt(fix16Sub(player.pos.x, HITBOX_OFF)),
		fix16ToInt(fix16Sub(player.pos.y, HITBOX_OFF)),
		TILE_ATTR(PAL1, 0, 0, 0));
	player.lives = 2;
	player.power = 0;
	SPR_setDepth(player.image, 6);
	SPR_setDepth(player.hitboxImage, 1);
}

#define PLAYER_SPEED FIX16(3)
#define PLAYER_SPEED_NORM FIX16(3 * NORM)

// collision

#define PLAYER_LIMIT_X FIX16(4)
#define PLAYER_LIMIT_W FIX16(GAME_W - 3)
#define PLAYER_LIMIT_Y FIX16(3)
#define PLAYER_LIMIT_H FIX16(GAME_H - 4)

static void checkPlayerBounds(){
	if(player.pos.x < PLAYER_LIMIT_X) player.pos.x = PLAYER_LIMIT_X;
	else if(player.pos.x > PLAYER_LIMIT_W) player.pos.x = PLAYER_LIMIT_W;
	if(player.pos.y < PLAYER_LIMIT_Y) player.pos.y = PLAYER_LIMIT_Y;
	else if(player.pos.y > PLAYER_LIMIT_H) player.pos.y = PLAYER_LIMIT_H;
}

s16 recoverClock;
#define RECOVER_INTERVAL 30
#define RECOVER_INTERVAL_HALF 15
#define RECOVER_MAX 180
static void updatePlayerHit(){
	if(player.hit){
		player.hit = FALSE;
		player.recovering = TRUE;
		player.pos.x = PLAYER_INIT_X;
		player.pos.y = PLAYER_INIT_Y;
		player.lives--;
		// XGM_startPlayPCM(random() % 2 < 1 ? SFX_EXPLOSION_1 : SFX_EXPLOSION_2, 1, SOUND_PCM_CH4);
		spawnExplosion(player.pos.x, player.pos.y, 0);
		if(player.lives < 0) gameOver = TRUE;
	}
	if(player.recovering && !gameOver){
		if(recoverClock % RECOVER_INTERVAL == 0){
			SPR_setVisibility(player.image, HIDDEN);
			SPR_setVisibility(player.hitboxImage, HIDDEN);
		} else if(recoverClock % RECOVER_INTERVAL == RECOVER_INTERVAL_HALF){
			SPR_setVisibility(player.image, VISIBLE);
			SPR_setVisibility(player.hitboxImage, VISIBLE);
		}
		recoverClock++;
		if(recoverClock >= RECOVER_MAX){
			SPR_setVisibility(player.image, VISIBLE);
			SPR_setVisibility(player.hitboxImage, VISIBLE);
			recoverClock = 0;
			player.recovering = FALSE;
		}
	}
}

// movement

bool movingL, movingR;
s16 playerMoveClock;

#define RESET_MOVE() if(movingL || movingR){ movingL = FALSE; movingR = FALSE; playerMoveClock = 0; }

static void animatePlayer(){
	if(movingL || movingR){
		if(playerMoveClock == 0){
			SPR_setFrame(player.image, 0);
			SPR_setAnim(player.image, movingL ? 1 : 2);
		} else if(playerMoveClock == 10) SPR_setFrame(player.image, 1);
	}	else {
		if(playerMoveClock == 0) SPR_setAnim(player.image, 0);
		if(playerMoveClock % 10 == 0) SPR_setFrame(player.image,
			playerMoveClock % 40 == 0 ? 0 : (playerMoveClock % 40 == 10 || playerMoveClock % 40 == 30 ? 1 : 2));
	}
}

static void movePlayer(){
	if(ctrl.left || ctrl.right || ctrl.up || ctrl.down){
		if(ctrl.left || ctrl.right){
			if(ctrl.left && !movingL){
				movingL = TRUE;
				movingR = FALSE;
				playerMoveClock = 0;
			} else if(ctrl.right && !movingR){
				movingR = TRUE;
				movingL = FALSE;
				playerMoveClock = 0;
			}
			if(ctrl.up || ctrl.down){
				player.vel.x = ctrl.left ? -PLAYER_SPEED_NORM : PLAYER_SPEED_NORM;
				player.vel.y = ctrl.up ? -PLAYER_SPEED_NORM : PLAYER_SPEED_NORM;
			} else {
				player.vel.x = ctrl.left ? -PLAYER_SPEED : PLAYER_SPEED;
				player.vel.y = 0;
			}
		} else if(ctrl.up){
			RESET_MOVE();
			player.vel.x = 0;
			player.vel.y = -PLAYER_SPEED;
		} else if(ctrl.down){
			RESET_MOVE();
			player.vel.x = 0;
			player.vel.y = PLAYER_SPEED;
		}
		if(player.vel.x != 0) player.pos.x = fix16Add(player.pos.x, player.vel.x);
		if(player.vel.y != 0) player.pos.y = fix16Add(player.pos.y, player.vel.y);
		checkPlayerBounds();
	} else RESET_MOVE();
	animatePlayer();
	playerMoveClock++;
}


// shooting

// shoot

#define PLAYER_BULLET_SPEED FIX16(10)
#define PLAYER_BULLET_SPEED_NORM FIX16(7.07)
#define PLAYER_BULLET_OFF FIX16(8)

static void spawnPlayerBullet(){
	struct bulletSpawner spawner = {
		.x = player.pos.x,
		.y = player.pos.y,
		.image = &playerBullet,
		.player = TRUE
	};
	spawner.vel.x = PLAYER_BULLET_SPEED;
	spawner.vel.y = 0;
	spawnBullet(spawner, EMPTY);
	XGM_startPlayPCM(SFX_PLAYER_SHOT, 0, SOUND_PCM_CH4);
}


static void shotPlayer(){
	if(player.shotClock >= SHOT_INTERVAL && ctrl.a) player.shotClock = 0;
	if(player.shotClock == 0) spawnPlayerBullet();
	player.shotClock++;
	if(player.shotClock >= CLOCK_LIMIT) player.shotClock = SHOT_INTERVAL;
}


// loop

bool isPressing;
void loadPlayer(){
	isPressing = TRUE;
	spawnPlayer();
}

void updatePlayer(){
	if(gameOver){
		player.pos.x = FIX16(-32);
		player.pos.y = PLAYER_INIT_Y;
		SPR_setVisibility(player.image, HIDDEN);
		SPR_setVisibility(player.hitboxImage, HIDDEN);
		if(isPressing){
			if(!ctrl.up && !ctrl.down && !ctrl.left && !ctrl.right && !ctrl.start && !ctrl.a && !ctrl.b && !ctrl.c) isPressing = FALSE;
		} else {
			if(ctrl.start || ctrl.a || ctrl.b || ctrl.c) SYS_hardReset();
		}
	} else {
		movePlayer();
		shotPlayer();
		updatePlayerHit();
		SPR_setPosition(player.image,
			fix16ToInt(fix16Sub(player.pos.x, PLAYER_OFF_X)),
			fix16ToInt(fix16Sub(player.pos.y, PLAYER_OFF_Y)) + 1);
		SPR_setPosition(player.hitboxImage,
			fix16ToInt(fix16Sub(player.pos.x, HITBOX_OFF)),
			fix16ToInt(fix16Sub(player.pos.y, HITBOX_OFF)));
		player.clock++;
		if(player.clock >= CLOCK_LIMIT) player.clock -= CLOCK_LIMIT;
	}
}