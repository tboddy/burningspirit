// enemies

// lifecycle

void spawnEnemy(struct enemySpawner spawner, void(*updater), void(*suicide)){
	s16 i = -1;
	for(s16 j = 0; j < ENEMY_COUNT; j++) if(!enemies[j].active && i == -1) i = j;
	if(i > -1){
		enemies[i].active = TRUE;
		enemies[i].off.x = FIX16(spawner.offX);
		enemies[i].off.y = FIX16(spawner.offY);
		enemies[i].pos.x = FIX16(spawner.x);
		enemies[i].pos.y = FIX16(spawner.y);
		enemies[i].last.x = FIX16(spawner.x);
		enemies[i].last.y = FIX16(spawner.y);
		enemies[i].dist = intToFix32(spawner.offX - 4);
		enemies[i].speed = spawner.speed;
		enemies[i].angle = spawner.angle;
		enemies[i].boss = spawner.boss;
		enemies[i].bonus = spawner.bonus;
		enemies[i].dead = FALSE;
		enemies[i].health = spawner.health ? spawner.health : 1;
		enemies[i].flipClock = 0;
		if(spawner.vel.x && spawner.vel.y){
			enemies[i].vel.x = spawner.vel.x;
			enemies[i].vel.y = spawner.vel.y;
		} else {
			enemies[i].vel.x = fix16Mul(cosFix16(spawner.angle), spawner.speed);
			enemies[i].vel.y = fix16Mul(sinFix16(spawner.angle), spawner.speed);
		}
		for(s16 j = 0; j < COUNT_INT; j++){
			enemies[i].bools[j] = spawner.bools[j];
			enemies[i].ints[j] = spawner.ints[j];
			enemies[i].fixes[j] = spawner.fixes[j];
		}
		enemies[i].updater = updater;
		enemies[i].suicide = suicide;
		enemies[i].clock = 0;
		enemies[i].seen = FALSE;
		
		enemies[i].image = SPR_addSprite(spawner.image,
			fix16ToInt(fix16Sub(enemies[i].pos.x, enemies[i].off.x)),
			fix16ToInt(fix16Sub(enemies[i].pos.y, enemies[i].off.y)),
			TILE_ATTR(PAL1, 0, 0, 0));

		SPR_setAnim(enemies[i].image, spawner.anim ? spawner.anim : 0);
		SPR_setFrame(enemies[i].image, spawner.frame ? spawner.frame : 0);
		SPR_setDepth(enemies[i].image, 5);
		SPR_setVisibility(enemies[i].image, HIDDEN);
	}
}


// helpers

void updateEnemyVel(s16 i){
	enemies[i].vel.x = fix16Mul(cosFix16(enemies[i].angle), enemies[i].speed);
	enemies[i].vel.y = fix16Mul(sinFix16(enemies[i].angle), enemies[i].speed);
}

#define FLIP_MOD 15
static void enemyAnimate(s16 i){
	// if(enemies[i].flipClock == 0){
	// 	if(enemies[i].last.x > enemies[i].pos.x){
	// 		enemies[i].flipClock = FLIP_MOD;
	// 		SPR_setAnim(enemies[i].image, 1);
	// 		SPR_setHFlip(enemies[i].image, 0);
	// 	} else if(enemies[i].last.x < enemies[i].pos.x){
	// 		enemies[i].flipClock = FLIP_MOD;
	// 		SPR_setAnim(enemies[i].image, 1);
	// 		SPR_setHFlip(enemies[i].image, 1);
	// 	} else {
	// 		enemies[i].flipClock = FLIP_MOD;
	// 		SPR_setAnim(enemies[i].image, 0);
	// 		SPR_setHFlip(enemies[i].image, 0);
	// 	}
	// }
	// if(enemies[i].flipClock > 0)
	// 	enemies[i].flipClock--;
}


// collide
fix32 enemyDist;
#define PLAYER_ENEMY_DIST FIX32(8)
static void collideEnemyWithPlayer(s16 i){
	if(!player.recovering && !enemies[i].bonus){
		enemyDist = getApproximatedDistance(
			fix16ToFix32(fix16Sub(enemies[i].pos.x, player.pos.x)),
			fix16ToFix32(fix16Sub(enemies[i].pos.y, player.pos.y)));
		if(enemyDist <= enemies[i].dist){
			if(!enemies[i].boss) enemies[i].health = 0;
			player.hit = TRUE;
			killBullets = TRUE;
		}
	}
}


// loop

#define ENEMY_LINE FIX16(GAME_W / 2)

u8 candyType = 1;

static void updateEnemy(s16 i){
	if(!enemies[i].bonus) enemyCount++;
	enemies[i].last.x = enemies[i].pos.x;
	enemies[i].last.y = enemies[i].pos.y;
	if(enemies[i].seen && (enemies[i].pos.x < fix16Sub(0, enemies[i].off.x) ||
		enemies[i].pos.x > fix16Add(FIX16(GAME_W), enemies[i].off.x) ||
		enemies[i].pos.y < fix16Sub(0, enemies[i].off.y) ||
		enemies[i].pos.y > fix16Add(FIX16(GAME_H), enemies[i].off.y))){
		enemies[i].dead = TRUE;
		enemies[i].suicide(i);
	} else {
		if(!enemies[i].seen &&
			enemies[i].pos.y >= fix16Sub(0, enemies[i].off.y) &&
			enemies[i].pos.x >= fix16Sub(0, enemies[i].off.x) &&
			enemies[i].pos.x <= fix16Add(FIX16(GAME_W), enemies[i].off.x)){
			enemies[i].seen = TRUE;
			SPR_setVisibility(enemies[i].image, VISIBLE);
		}
		enemies[i].pos.x = fix16Add(enemies[i].pos.x, enemies[i].vel.x);
		enemies[i].pos.y = fix16Add(enemies[i].pos.y, enemies[i].vel.y);
		if(enemies[i].seen){
			enemies[i].updater(i);
			collideEnemyWithPlayer(i);
			if(!enemies[i].boss) enemyAnimate(i);
			enemies[i].clock++;
			if(enemies[i].clock >= CLOCK_LIMIT) enemies[i].clock -= CLOCK_LIMIT;
			SPR_setPosition(enemies[i].image,
				fix16ToInt(fix16Sub(enemies[i].pos.x, enemies[i].off.x)),
				fix16ToInt(fix16Sub(enemies[i].pos.y, enemies[i].off.y)));
		}
		if(enemies[i].health <= 0){
			enemies[i].suicide(i);
			enemies[i].dead = TRUE;
			SND_startPlayPCM_XGM(SFX_EXPLOSION_2, 15, SOUND_PCM_CH2);
			if(!enemies[i].boss && !player.hit && !enemies[i].bonus){
				score += enemies[i].pos.x < ENEMY_LINE ? 750 : 500;
				spawnPowerup(enemies[i].pos.x, enemies[i].pos.y, candyType);
			}
			candyType++;
			if(candyType >= 5) candyType = 1;
			// SND_startPlayPCM_XGM(SFX_EXPLOSION_2, 15, SOUND_PCM_CH2);
		}
	}
	if(enemies[i].dead){
		if(!enemies[i].bonus) enemyCount--;
		enemies[i].active = FALSE;
		SPR_releaseSprite(enemies[i].image);
	}
}

void updateEnemies(){
	enemyCount = 0;
	for(s16 i = 0; i < ENEMY_COUNT; i++) if(enemies[i].active) updateEnemy(i);
}