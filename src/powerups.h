// powerups

#define POWERUP_OFF 8
#define POWERUP_SPEED FIX16(-1.5)
#define POWERUP_MAX FIX16(1.5)


// lifecycle

void spawnPowerup(fix16 x, fix16 y, u8 type){
	s16 i = -1;
	for(s16 j = 0; j < POWERUP_COUNT; j++) if(!powerups[j].active && i == -1) i = j;
	powerups[i].active = TRUE;
	powerups[i].pos.x = x;
	powerups[i].pos.y = y;
	powerups[i].speed = POWERUP_SPEED;
	powerups[i].type = type;
	powerups[i].found = FALSE;
	powerups[i].image = SPR_addSprite(&candy, fix16ToInt(x) - POWERUP_OFF, fix16ToInt(y) - POWERUP_OFF, TILE_ATTR(PAL1, 0, 0, 0));
	SPR_setAnim(powerups[i].image, type);
}

#define POWERUP_MOD FIX16(0.25)
#define POWERUP_LIMIT_X FIX16(-8) 
#define POWERUP_LIMIT FIX16(GAME_H + 8) 

static void killPowerup(s16 i){
	powerups[i].active = FALSE;
	SPR_releaseSprite(powerups[i].image);
}

#define POWERUP_COLLIDE FIX32(16)
fix32 powerupDist;

// player

#define WEAPON_POWERUP_VEL FIX16(0.75)
static void updateWeaponPowerup(s16 i){
	powerupDist = getApproximatedDistance(
		fix16ToFix32(fix16Sub(player.pos.x, powerups[i].pos.x)),
		fix16ToFix32(fix16Sub(player.pos.y, powerups[i].pos.y)));
	powerups[i].pos.x = fix16Sub(powerups[i].pos.x, WEAPON_POWERUP_VEL);
	if(powerupDist < POWERUP_COLLIDE){
		killPowerup(i);
		if(player.power < 2) player.power++;
	}
}

// bonus
static void updateBonusPowerup(s16 i){}


// candy


static void collidePowerup(s16 i){
	powerupDist = getApproximatedDistance(
		fix16ToFix32(fix16Sub(player.pos.x, powerups[i].pos.x)),
		fix16ToFix32(fix16Sub(player.pos.y, powerups[i].pos.y)));
	if(powerupDist < POWERUP_COLLIDE){
		killPowerup(i);
		score += 250;
	}
}

static void updateCandy(s16 i){
	if(powerups[i].found){
		powerups[i].vel = hone(powerups[i].pos, player.pos, powerups[i].speed, 0);
		powerups[i].pos.x = fix16Add(powerups[i].pos.x, powerups[i].vel.x);
		powerups[i].pos.y = fix16Add(powerups[i].pos.y, powerups[i].vel.y);
		powerups[i].speed = fix16Add(powerups[i].speed, FIX16(0.75));
		collidePowerup(i);
	} else {
		powerups[i].pos.y = fix16Add(powerups[i].pos.y, powerups[i].speed);
		if(powerups[i].clock > 0 && powerups[i].clock % 10 == 0 && powerups[i].speed < POWERUP_MAX) powerups[i].speed = fix16Add(powerups[i].speed, POWERUP_MOD);
		if(!ctrl.a) powerups[i].found = TRUE;
	}
}


// loop

static void updatePowerup(s16 i){
	if(powerups[i].type == 0) updateBonusPowerup(i);
	else if(powerups[i].type >= 1 && powerups[i].type < 5) updateCandy(i);
	SPR_setPosition(powerups[i].image, fix16ToInt(powerups[i].pos.x) - POWERUP_OFF, fix16ToInt(powerups[i].pos.y) - POWERUP_OFF);
	powerups[i].clock++;
	if(powerups[i].pos.y >= POWERUP_LIMIT || powerups[i].pos.x <= POWERUP_LIMIT_X) killPowerup(i);
}

void updatePowerups(){
	for(s16 i = 0; i < POWERUP_COUNT; i++) if(powerups[i].active) updatePowerup(i);
}