// bullets

#define BULLET_OFF 4
#define BULLET_OFF_BIG 8
#define BULLET_OFF_HUGE 16

#define BULLET_DIST FIX32(4)
#define BULLET_DIST_BIG FIX32(8)
#define BULLET_DIST_HUGE FIX32(16)


// lifecycle

void spawnBullet(struct bulletSpawner spawner, void(*updater)){
	s16 i = -1;
	for(s16 j = 0; j < BULLET_COUNT; j++) if(!bullets[j].active && i == -1) i = j;
	if(i > -1 && (spawner.player || (player.invincibleClock < INVINCIBLE_LIMIT))){
		bullets[i].active = TRUE;
		bullets[i].pos.x = spawner.x;
		bullets[i].pos.y = spawner.y;
		bullets[i].dist = spawner.player ? BULLET_DIST_HUGE : (spawner.huge ? BULLET_DIST_HUGE : (spawner.big ? BULLET_DIST_BIG : BULLET_DIST));
		bullets[i].distCheck = fix32Sub(bullets[i].dist, FIX32(2));
		bullets[i].speed = spawner.speed;
		bullets[i].angle = spawner.angle;
		bullets[i].player = spawner.player;
		bullets[i].clock = 0;
		bullets[i].dead = FALSE;
		bullets[i].big = spawner.big;
		bullets[i].huge = spawner.huge;
		bullets[i].light = spawner.light;
		for(u8 j = 0; j < COUNT_INT; j++){
			bullets[i].bools[j] = spawner.bools[j];
			bullets[i].ints[j] = spawner.ints[j];
			bullets[i].fixes[j] = spawner.fixes[j];
		}
		if(spawner.vel.x || spawner.vel.y){
			bullets[i].vel.x = spawner.vel.x;
			bullets[i].vel.y = spawner.vel.y;
			if(bullets[i].vel.y < 0 && !bullets[i].player && bullets[i].pos.y < player.pos.y) bullets[i].vel.y = -bullets[i].vel.y;
		} else {
			bullets[i].vel.x = fix16Mul(cosFix16(spawner.angle), spawner.speed);
			bullets[i].vel.y = fix16Mul(sinFix16(spawner.angle), spawner.speed);
		}
		bullets[i].updater = updater;
		bullets[i].image = SPR_addSprite(spawner.image,
			fix16ToInt(fix16Sub(bullets[i].pos.x, FIX16(spawner.player ? BULLET_OFF_BIG : (spawner.huge ? BULLET_OFF_HUGE : (spawner.big ? BULLET_OFF_BIG : BULLET_OFF))))),
			fix16ToInt(fix16Sub(bullets[i].pos.y, FIX16(spawner.player ? BULLET_OFF_BIG : (spawner.huge ? BULLET_OFF_HUGE : (spawner.big ? BULLET_OFF_BIG : BULLET_OFF))))),
			TILE_ATTR(PAL1, 0, 0, 0));
		if(spawner.light) SPR_setAnim(bullets[i].image, 1);
		SPR_setDepth(bullets[i].image, spawner.player ? 7 : (spawner.top ? 3 : 4));
	}
}

static void killBullet(s16 i){
	bullets[i].active = FALSE;
	SPR_releaseSprite(bullets[i].image);
	if(bullets[i].dead){
		spawnExplosion(bullets[i].pos.x, bullets[i].pos.y, 0);
		bullets[i].dead = FALSE;
	}
}

void updateBulletVel(s16 i){
	bullets[i].vel.x = fix16Mul(cosFix16(bullets[i].angle), bullets[i].speed);
	bullets[i].vel.y = fix16Mul(sinFix16(bullets[i].angle), bullets[i].speed);
}


// collision

fix32 bulletDist, eBulletDist;

static void collideBulletWithEnemy(s16 i){
	for(s16 j = 0; j < ENEMY_COUNT; j++) if(enemies[j].active && enemies[j].pos.x <= FIX16(GAME_W - 8)){
		bulletDist = getApproximatedDistance(
			fix16ToFix32(fix16Sub(enemies[j].pos.x, bullets[i].pos.x)),
			fix16ToFix32(fix16Sub(enemies[j].pos.y, bullets[i].pos.y)));
		if(bulletDist <= fix32Add(enemies[j].dist, bullets[i].dist)){
			spawnExplosion(bullets[i].pos.x, bullets[i].pos.y, 1);
			killBullet(i);
			if(enemies[j].clock > 0) enemies[j].health--;
		}
	}
}

#define PLAYER_BULLET_CHECK FIX32(1)

static void collideBulletWithPlayer(s16 i){
	eBulletDist = getApproximatedDistance(
		fix16ToFix32(fix16Sub(player.pos.x, bullets[i].pos.x)),
		fix16ToFix32(fix16Sub(player.pos.y, bullets[i].pos.y)));
	if(eBulletDist <= bullets[i].distCheck){
		player.hit = TRUE;
		killBullets = TRUE;
		spawnExplosion(player.pos.x, player.pos.y, 0);
	}
}

#define BULLET_LIMIT FIX16(0 - BULLET_OFF)
#define BULLET_LIMIT_W FIX16(GAME_W + BULLET_OFF)
#define BULLET_LIMIT_H FIX16(GAME_H + BULLET_OFF)

#define BULLET_LIMIT_BIG FIX16(0 - BULLET_OFF_BIG)
#define BULLET_LIMIT_W_BIG FIX16(GAME_W + BULLET_OFF_BIG)
#define BULLET_LIMIT_H_BIG FIX16(GAME_H + BULLET_OFF_BIG)

#define BULLET_LIMIT_HUGE FIX16(0 - BULLET_OFF_HUGE)
#define BULLET_LIMIT_W_HUGE FIX16(GAME_W + BULLET_OFF_HUGE)
#define BULLET_LIMIT_H_HUGE FIX16(GAME_H + BULLET_OFF_HUGE)

static void collideBullet(s16 i){
	if(bullets[i].pos.x < (bullets[i].huge ? BULLET_LIMIT_HUGE : (bullets[i].big ? BULLET_LIMIT_BIG : BULLET_LIMIT)) ||
		bullets[i].pos.x > (bullets[i].huge ? BULLET_LIMIT_W_HUGE : (bullets[i].big ? BULLET_LIMIT_W_BIG : BULLET_LIMIT_W)) ||
		bullets[i].pos.y < (bullets[i].huge ? BULLET_LIMIT_HUGE : (bullets[i].big ? BULLET_LIMIT_BIG : BULLET_LIMIT)) ||
		bullets[i].pos.y > (bullets[i].huge ? BULLET_LIMIT_H_HUGE : (bullets[i].big ? BULLET_LIMIT_H_BIG : BULLET_LIMIT_H))){
		killBullet(i);
	} else if(bullets[i].player) collideBulletWithEnemy(i);
	else if(!player.recovering && !gameOver) collideBulletWithPlayer(i);
}


// loop

static void updateBullet(s16 i){
	bullets[i].pos.x = fix16Add(bullets[i].pos.x, bullets[i].vel.x);
	bullets[i].pos.y = fix16Add(bullets[i].pos.y, bullets[i].vel.y);
	bullets[i].updater(i);
	collideBullet(i);
	SPR_setPosition(
		bullets[i].image,
		fix16ToInt(fix16Sub(bullets[i].pos.x, FIX16(bullets[i].player ? BULLET_OFF_BIG : (bullets[i].huge ? BULLET_OFF_HUGE : (bullets[i].big ? BULLET_OFF_BIG : BULLET_OFF))))),
		fix16ToInt(fix16Sub(bullets[i].pos.y, FIX16(bullets[i].player ? BULLET_OFF_BIG : (bullets[i].huge ? BULLET_OFF_HUGE : (bullets[i].big ? BULLET_OFF_BIG : BULLET_OFF))))));
	bullets[i].clock++;
}

void updateBullets(){
	if(killBullets){
		for(s16 i = 0; i < BULLET_COUNT; i++) if(bullets[i].active){
			if(i % 3 == 0) bullets[i].dead = TRUE;
			killBullet(i);
		}
		killBullets = FALSE;
	} else {
		for(s16 i = 0; i < BULLET_COUNT; i++) if(bullets[i].active) updateBullet(i);
	}
}