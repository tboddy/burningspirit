// explosions

#define EXPLOSION_OFFSET FIX16(16)
#define EXPLOSION_TIME 20

void spawnExplosion(fix16 x, fix16 y, u8 type){
	s16 i = -1;
	for(s16 j = 0; j < EXPLOSION_COUNT; j++) if(!explosions[j].active && i == -1) i = j;
	explosions[i].active = TRUE;
	explosions[i].clock = 0;
	explosions[i].pos.x = x;
	explosions[i].pos.y = y;
	explosions[i].type = type;
	explosions[i].image = SPR_addSprite(&explosion,
		fix16ToInt(fix16Sub(x, EXPLOSION_OFFSET)),
		fix16ToInt(fix16Sub(y, EXPLOSION_OFFSET)),
		TILE_ATTR(PAL1, 1, 0, 0));
	SPR_setAnim(explosions[i].image, type);
	SPR_setDepth(explosions[i].image, 3);
}

static void updateExplosion(s16 i){
	if(explosions[i].clock >= EXPLOSION_TIME){
		explosions[i].active = FALSE;
		SPR_releaseSprite(explosions[i].image);
		// if(explosions[i].type == 2) spawnPowerup(explosions[i].pos.x, explosions[i].pos.y, 0);
	}
	explosions[i].clock++;
}

void updateExplosions(){
	for(s16 i = 0; i < EXPLOSION_COUNT; i++) if(explosions[i].active) updateExplosion(i);
}