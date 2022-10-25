// stage


s16 stageClock, nextClock;


// groups

static void waveSine(bool bottom){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1.5),
		.x = GAME_W + 8,
		.y = GAME_H / 2,
		.image = &turnip,
		.anim = bottom ? 0 : 1,
		.offX = 12,
		.offY = 12,
		.health = 1
	};
	spawner.fixes[0] = FIX16(spawner.y);
	spawner.fixes[1] = FIX16(24);
	spawner.fixes[2] = FIX16(48);
	spawner.fixes[3] = FIX16(0.08);
	spawner.bools[0] = bottom;
	void updater(s16 i){
		enemies[i].pos.y = fix16Sub(enemies[i].fixes[0], fix16Mul(sinFix16(enemies[i].fixes[1]), enemies[i].fixes[2]));
		enemies[i].fixes[1] = enemies[i].bools[0] ? fix16Add(enemies[i].fixes[1], enemies[i].fixes[3]) : fix16Sub(enemies[i].fixes[1], enemies[i].fixes[3]);
	}
	for(u8 i = 0; i < 5; i++){
		spawnEnemy(spawner, updater, EMPTY);
		spawner.x += 32;
		spawner.frame = i % 4;
	}
}

static void waveBig1(){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1.5),
		.x = GAME_W + 16,
		.y = GAME_H / 2,
		.image = &bakebake,
		.offX = 16,
		.offY = 16,
		.health = 3
	};
	void updater(s16 i){
		if(enemies[i].clock == 15){
			struct bulletSpawner spawner = {
				.x = enemies[i].pos.x,
				.y = enemies[i].pos.y,
				.image = &bigBullet,
				.angle = 384,
				.speed = FIX16(1.5),
				.big = TRUE
			};
			for(u8 j = 0; j < 5; j++){
				spawnBullet(spawner, EMPTY);
				spawner.angle += 64;
			}
			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
		} else if(enemies[i].clock == 60){
			enemies[i].speed = FIX16(1.25);
			updateEnemyVel(i);
		} else if(enemies[i].clock == 90){
			enemies[i].speed = FIX16(1);
			updateEnemyVel(i);
		}
	}
	spawnEnemy(spawner, updater, EMPTY);
}

static void waveBig2(bool top){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1.25),
		.y = top ? GAME_H / 3 : GAME_H / 3 * 2,
		.x = GAME_W + 48,
		.image = &bakebake,
		.offX = 16,
		.offY = 16,
		.health = 2,
		.anim = top ? 1 : 0
	};
	spawner.bools[0] = top;
	void updater(s16 i){
		if(enemies[i].clock == 5){
			velPos.x = player.pos.x;
			velPos.y = player.pos.y;
		}
		if(enemies[i].clock < 80 && enemies[i].clock % 15 == 5){
			struct bulletSpawner spawner = {
				.x = enemies[i].pos.x,
				.y = enemies[i].pos.y,
				.speed = FIX16(1.75),
				.image = &smallBullet,
				.light = enemies[i].clock % 30 == 0,
				.top = TRUE
			};
			spawner.vel = hone(enemies[i].pos, velPos, spawner.speed, 56);
			spawnBullet(spawner, EMPTY);
			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
		}
		if(enemies[i].bools[0] && enemies[i].clock == 40){
			struct bulletSpawner spawner = {
				.x = enemies[i].pos.x,
				.y = enemies[i].pos.y,
				.image = &bigBullet,
				.angle = 320,
				.speed = FIX16(1.5),
				.big = TRUE
			};
			for(u8 j = 0; j < 4; j++){
				spawnBullet(spawner, EMPTY);
				spawner.angle += 64;
			}
			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
		}
		if(enemies[i].clock == 60){
			enemies[i].angle += enemies[i].bools[0] ? -16 : 16;
			updateEnemyVel(i);
		} else if(enemies[i].clock == 90){
			enemies[i].angle += enemies[i].bools[0] ? -16 : 16;
			updateEnemyVel(i);
		}
	}
	spawnEnemy(spawner, updater, EMPTY);
}

static void waveBig3(){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1.25),
		.x = GAME_W + 16,
		.y = GAME_H / 2,
		.image = &bakebake,
		.offX = 16,
		.offY = 16,
		.health = 4
	};
	void updater(s16 i){
		if(enemies[i].clock < 80 && enemies[i].clock % 30 == 5){

			struct bulletSpawner spawner = {
				.x = enemies[i].pos.x,
				.y = enemies[i].pos.y,
				.speed = FIX16(2)
			};

			if(enemies[i].clock == 35){
				spawner.image = &smallBullet;
				spawner.angle = 352;
			} else {
				spawner.image = &bigBullet;
				spawner.big = TRUE;
				spawner.angle = 384;
			}
			for(u8 j = 0; j < (enemies[i].clock == 35 ? 6 : 5); j++){
				spawnBullet(spawner, EMPTY);
				spawner.angle += 64;
			}
			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
		}

		if(enemies[i].clock == 60){
			enemies[i].speed = FIX16(1);
			updateEnemyVel(i);
		} else if(enemies[i].clock == 90){
			enemies[i].speed = FIX16(0.75);
			updateEnemyVel(i);
		}

	}
	spawnEnemy(spawner, updater, EMPTY);
}

static void waveSwarm(u8 count){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(2),
		.x = GAME_W + 16,
		.image = &turnip,
		.offX = 12,
		.offY = 12,
		.health = 1
	};
	void updater(s16 i){
		if(enemies[i].clock == 45){
			enemies[i].speed = FIX16(1.75);
			updateEnemyVel(i);
		} else if(enemies[i].clock == 90){
			enemies[i].speed = FIX16(1.5);
			updateEnemyVel(i);
		}
	}
	for(u8 i = 0; i < count; i++){
		spawner.y = random() % (GAME_H / 2 - 32);
		spawner.y += (i % 2 == 0) ? (GAME_H / 2) : 32;
		spawner.x += 32;
		spawner.anim = i % 4 < 2 ? 1 : 0;
		spawner.angle = 512;
		spawner.frame = i % 4;
		if(i % 2 == 0) spawner.angle += random() % 16; // ...why not working with negative random?
		else spawner.angle -= random() % 16;
		spawnEnemy(spawner, updater, EMPTY);
	}
}

static void waveBig4(bool bottom, bool second){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1.5),
		.y = bottom ? (GAME_H / 4 * 3) : (GAME_H / 4),
		.x = GAME_W + 16,
		.image = &bakebake,
		.offX = 16,
		.offY = 16,
		.health = 3
	};
	if(bottom) spawner.x += 32;
	spawner.bools[0] = bottom;
	spawner.bools[1] = second;
	void updater(s16 i){

		if(enemies[i].clock == 10){
			struct bulletSpawner spawner = {
				.x = enemies[i].pos.x,
				.y = enemies[i].pos.y,
				.image = &bigBullet,
				.big = TRUE,
				.speed = FIX16(1.5),
				.angle = 32 + 256,
				.light = enemies[i].bools[0]
			};
			if(enemies[i].bools[0]) spawner.angle = 1024 - spawner.angle;
			for(u8 j = 0; j < 5; j++){
				spawnBullet(spawner, EMPTY);
				spawner.angle += enemies[i].bools[0] ? -64 : 64;
			}
			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
		} else if(enemies[i].clock >= 15 && enemies[i].clock < 90 && enemies[i].clock % 15 == 0 && enemies[i].bools[1]){
			struct bulletSpawner spawner = {
				.x = enemies[i].pos.x,
				.y = enemies[i].pos.y,
				.image = &smallBullet,
				.speed = FIX16(2),
				.light = !enemies[i].bools[0]
			};
			if(enemies[i].clock == 15){
				velPos = hone(enemies[i].pos, player.pos, spawner.speed, 0);
				enemies[i].fixes[0] = velPos.x;
				enemies[i].fixes[1] = velPos.y;
			}
			spawner.vel.x = enemies[i].fixes[0];
			spawner.vel.y = enemies[i].fixes[1];
			// spawner.vel = velPos;
			spawnBullet(spawner, EMPTY);
			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
		}

		if(enemies[i].clock == 45){
			enemies[i].speed = FIX16(1.25);
			enemies[i].angle += enemies[i].bools[0] ? 16 : -16;
			updateEnemyVel(i);
		} else if(enemies[i].clock == 90){
			enemies[i].speed = FIX16(1);
			enemies[i].angle += enemies[i].bools[0] ? 16 : -16;
			updateEnemyVel(i);
		}

	}
	spawnEnemy(spawner, updater, EMPTY);
}

static void waveBig5(bool bottom){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1.25),
		.x = GAME_W	+ 16,
		.y = GAME_H / 4,
		.image = &bakebake,
		.offX = 16,
		.offY = 16,
		.health = 2
	};
	if(bottom){
		spawner.y = GAME_H / 4 * 3;
		spawner.x += 32;
		spawner.anim = 1;
		spawner.frame = 2;
	}
	spawner.bools[0] = bottom;
	void updater(s16 i){
		if(enemies[i].clock > 0 && enemies[i].clock % 60 == 5 && enemies[i].clock < 180){
			struct bulletSpawner spawner = {
				.x = enemies[i].pos.x,
				.y = enemies[i].pos.y,
				.image = &bigBullet,
				.big = TRUE,
				.angle = enemies[i].bools[0] ? 768 : 256,
				.speed = FIX16(2),
				.light = enemies[i].bools[0]
			};
			spawner.angle += enemies[i].bools[0] ? enemies[i].ints[0] : -enemies[i].ints[0];
			spawner.bools[0] = enemies[i].bools[0];
			void updater(s16 j){
				if(bullets[j].clock > 30 && bullets[j].clock % 16 == 0){
					bullets[j].angle += bullets[j].bools[0] ? -32 : 32;
					updateBulletVel(j);
				}
			}
			for(u8 j = 0; j < 5; j++){
				if(j > 0) spawnBullet(spawner, updater);
				spawner.angle += enemies[i].bools[0] ? -64 : 64;
			}
			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
			enemies[i].ints[0] += 56;
		}
		if(enemies[i].clock == 60){
			enemies[i].angle += enemies[i].bools[0] ? 16 : -16;
			enemies[i].speed = FIX16(1);
			updateEnemyVel(i);
		} else if(enemies[i].clock == 120){
			enemies[i].angle += enemies[i].bools[0] ? 16 : -16;
			enemies[i].speed = FIX16(0.75);
			updateEnemyVel(i);
		}
	}
	spawnEnemy(spawner, updater, EMPTY);
}

static void waveBig6(bool bottom){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1.25),
		.x = GAME_W	+ 16,
		.y = GAME_H / 4,
		.image = &bakebake,
		.offX = 16,
		.offY = 16,
		.health = 2
	};
	if(bottom){
		spawner.y = GAME_H / 4 * 3;
		spawner.x += 32;
		spawner.anim = 1;
		spawner.frame = 2;
	}
	spawner.bools[0] = bottom;
	void updater(s16 i){
		if(enemies[i].clock < 150 && enemies[i].clock % 8 == 0){
			struct bulletSpawner spawner = {
				.x = enemies[i].pos.x,
				.y = enemies[i].pos.y,
				.speed = FIX16(2),
				.big = TRUE,
				.image = &bigBullet,
				.angle = 256 - 32 + random() % 320 + 128
			};
			if(enemies[i].bools[0]) spawner.angle = 512 - 32 + random() % 320 - 128;
			if(enemies[i].clock % 16 == 0){
				spawner.speed = FIX16(2.5);
				spawner.image = &smallBullet;
				spawner.top = TRUE;
			}
			spawnBullet(spawner, EMPTY);
			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
		}
		if(enemies[i].clock == 60){
			enemies[i].angle += enemies[i].bools[0] ? 8 : -8;
			enemies[i].speed = FIX16(1);
			updateEnemyVel(i);
		} else if(enemies[i].clock == 120){
			enemies[i].angle += enemies[i].bools[0] ? 8 : -8;
			enemies[i].speed = FIX16(0.75);
			updateEnemyVel(i);
		}
	}
	spawnEnemy(spawner, updater, EMPTY);
}

static void waveFormation(bool bottom){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1.5),
		.x = GAME_W + 64,
		.y = bottom ? GAME_H / 4 * 3 : GAME_H / 4,
		.image = &turnip,
		.offX = 12,
		.offY = 12,
		.health = 1,
		.anim = bottom ? 1 : 0
	};
	if(bottom){
		spawner.x += 32;
		spawner.bools[0] = TRUE;
	}
	void updater(s16 i){

		if(enemies[i].clock == 10){
			struct bulletSpawner spawner = {
				.x = enemies[i].pos.x,
				.y = enemies[i].pos.y,
				.image = &smallBullet,
				.light = TRUE,
				.speed = FIX16(2)
			};
			spawner.vel = hone(enemies[i].pos, player.pos, spawner.speed, 16);
			spawnBullet(spawner, updater);
			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
		}
		if(enemies[i].clock == 60){
			// enemies[i].angle += 32;
			enemies[i].speed = FIX16(1.25);
			updateEnemyVel(i);
		} else if(enemies[i].clock == 90){
			// enemies[i].angle += 32;
			enemies[i].speed = FIX16(1);
			updateEnemyVel(i);
		}

	}
	for(u8 i = 0; i < 4; i++){
		spawnEnemy(spawner, updater, EMPTY);
		spawner.y += bottom ? -32 : 32;
		spawner.x += 32;
	}
}


// bosses

#define BOSS_MOD_Y FIX16(40)
#define BOSS_MOD_X FIX16(12)
fix16 bossLimitY, bossLimitH, bossLimitX, bossLimitW;

static void bossIntro(s16 i){
	enemies[i].last.x = enemies[i].pos.x;
	enemies[i].clock = -1;
	if(clock % 32 == 0){
		enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.25));
		if(enemies[i].speed <= 0){
			bossLimitY = fix16Sub(enemies[i].pos.y, BOSS_MOD_Y);
			bossLimitH = fix16Add(enemies[i].pos.y, BOSS_MOD_Y);
			bossLimitX = fix16Sub(enemies[i].pos.x, BOSS_MOD_X);
			bossLimitW = fix16Add(enemies[i].pos.x, BOSS_MOD_X);
			// enemies[i].speed = FIX16(0.5);
			enemies[i].angle = 224;
			enemies[i].bools[0] = TRUE;
			enemies[i].clock = -1;
		}
		updateEnemyVel(i);
	}
}

static void bossMove(s16 i){
	if(enemies[i].pos.x > bossLimitW || enemies[i].pos.x < bossLimitX){
		enemies[i].pos.x = enemies[i].pos.x > bossLimitW ? bossLimitW : bossLimitX;
		enemies[i].angle = 512 - enemies[i].angle;
		updateEnemyVel(i);
	}
	if(enemies[i].pos.y > bossLimitH || enemies[i].pos.y < bossLimitY){
		enemies[i].pos.y = enemies[i].pos.y > bossLimitH ? bossLimitH : bossLimitY;
		enemies[i].angle = 1024 - enemies[i].angle;
		updateEnemyVel(i);
	}
	if(enemies[i].clock % 120 == 119){
		enemies[i].speed = enemies[i].speed == 0 ? FIX16(0.5) : 0;
		updateEnemyVel(i);
	}
}

static void waveMidboss1(){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1),
		.x = GAME_W + 24,
		.y = GAME_H / 2,
		.image = &eye,
		.offX = 24,
		.offY = 24,
		.boss = TRUE,
		.health = 40
	};
	bossMax = spawner.health;
	void updater(s16 i){
		if(enemies[i].bools[0]){
			bossHealth = enemies[i].health;
			bossMove(i);
			if(enemies[i].health > 20){
				// ring
				if(enemies[i].clock % 90 == 0){
					struct bulletSpawner spawner = {
						.x = enemies[i].pos.x,
						.y = enemies[i].pos.y,
						.image = &bigBullet,
						.big = TRUE,
						.speed = FIX16(2),
						.angle = 256 + 80 + random() % 32,
						.light = TRUE
					};
					for(u8 j = 3; j < 15; j++){
						if(j % 3 < 2) spawnBullet(spawner, EMPTY);
						spawner.angle += 32;
					}
					XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
				}
				// puke
				else if(enemies[i].clock % 20 == 0){
					struct bulletSpawner spawner = {
						.x = FIX16(GAME_W - 32),
						.y = FIX16(enemies[i].clock % 180 < 90 ? 48 : GAME_H - 48),
						.image = &bigBullet,
						.big = TRUE,
						.top = TRUE,
						.speed = FIX16(2.5)
					};
					velPos.x = spawner.x;
					velPos.y = spawner.y;
					spawner.vel = hone(velPos, player.pos, spawner.speed, 32);
					spawnBullet(spawner, EMPTY);
					XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
					spawnExplosion(spawner.x, spawner.y, 0);
				}
			} else {
				if(!enemies[i].bools[7]){
					killBullets = TRUE;
					enemies[i].bools[7] = TRUE;
					enemies[i].clock = -60;
					SND_startPlayPCM_XGM(SFX_EXPLOSION_2, 15, SOUND_PCM_CH2);
				} else if(enemies[i].clock >= 0){
					// swash
					if(enemies[i].clock % 8 == 0){
						struct bulletSpawner spawner = {
							.x = enemies[i].pos.x,
							.y = enemies[i].pos.y,
							.image = &smallBullet,
							.speed = FIX16(2),
							.light = TRUE,
							.top = TRUE,
							.angle = enemies[i].ints[0] + 256
						};
						if(enemies[i].clock % 16 == 8){
							spawner.image = &bigBullet;
							spawner.big = TRUE;
						}
						void updater(s16 j){
							if(bullets[j].bools[0] && bullets[j].clock == 15){
								bullets[j].angle = 256 + 64 + random() % 384;
								bullets[j].speed = FIX16(2.5);
								updateBulletVel(j);
							} else if(!bullets[j].bools[0] && bullets[j].clock >= 0 && bullets[j].clock % 10 == 0){
								bullets[j].speed = fix16Sub(bullets[j].speed, FIX16(0.25));
								if(bullets[j].speed <= 0){
									bullets[j].speed = 0;
									bullets[j].clock = -1;
									bullets[j].bools[0] = TRUE;
								}
								updateBulletVel(j);
							}	
						}
						spawnBullet(spawner, updater);
						XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
						enemies[i].ints[0] += enemies[i].bools[4] ? -64 : 56;
						if(enemies[i].ints[0] > 512 || enemies[i].ints[0] < 0) enemies[i].bools[4] = !enemies[i].bools[4];
					}
					// balls
					if(enemies[i].clock % 45 == 0 && enemies[i].clock > 0){
						struct bulletSpawner spawner = {
							.x = enemies[i].pos.x,
							.y = enemies[i].pos.y,
							.image = &hugeBullet,
							.speed = FIX16(2),
							.huge = TRUE
						};
						spawner.vel = hone(enemies[i].pos, player.pos, spawner.speed, 16);
						spawnBullet(spawner, EMPTY);
						XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
					}
				}
			}
		} else bossIntro(i);
	}
	void suicide(s16 i){
		stageClock = -60;
		score += 2500;
		bossHealth = 0;
		killBullets = TRUE;
	}
	spawnEnemy(spawner, updater, suicide);
}

static void waveMidboss2(){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1),
		.x = GAME_W + 24,
		.y = GAME_H / 2,
		.image = &eye,
		.offX = 24,
		.offY = 24,
		.boss = TRUE,
		.health = 60
	};
	bossMax = spawner.health;
	void updater(s16 i){
		if(enemies[i].bools[0]){
			bossHealth = enemies[i].health;
			bossMove(i);
			if(enemies[i].health > 40){
				// laser
				if(enemies[i].clock % 30 == 5){
					struct bulletSpawner spawner = {
						.x = enemies[i].pos.x,
						.y = enemies[i].pos.y,
						.image = &hugeBullet,
						.huge = TRUE,
						.speed = FIX16(2.5),
						.angle = 512 - 40 + random() % 80,
						.light = TRUE
					};
					spawnBullet(spawner, EMPTY);
					XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
				}
				// homing
				if(enemies[i].clock % 15 == 10){
					struct bulletSpawner spawner = {
						.x = FIX16(GAME_W - 48 + random() % 32),
						.y = FIX16(32 + random() % (GAME_H - 64)),
						.image = &bigBullet,
						.speed = FIX16(2),
						.angle = 512,
						.top = TRUE
					};
					spawner.fixes[0] = spawner.speed;
					void updater(s16 j){
						if(bullets[j].clock == 10){
							bullets[j].speed = 0;
							updateBulletVel(j);
						}
						else if(bullets[j].clock >= 20 && bullets[j].clock % 20 == 0 && bullets[j].clock < 60)
							bullets[j].vel = hone(bullets[j].pos, player.pos, bullets[j].fixes[0], 8);
					}
					spawnBullet(spawner, updater);
					XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
					spawnExplosion(spawner.x, spawner.y, 0);
				}
			} else if(enemies[i].health > 20){
				if(!enemies[i].bools[7]){
					killBullets = TRUE;
					enemies[i].bools[7] = TRUE;
					enemies[i].clock = -60;
					SND_startPlayPCM_XGM(SFX_EXPLOSION_2, 15, SOUND_PCM_CH2);
				} else if(enemies[i].clock >= 0){
					// ring
					if(enemies[i].clock % 5 == 0){
						struct bulletSpawner spawner = {
							.x = enemies[i].pos.x,
							.y = enemies[i].pos.y,
							.image = &bigBullet,
							.light = TRUE,
							.speed = FIX16(2.5),
							.top = TRUE,
							.big = TRUE,
							.angle = random() % 128 + (enemies[i].clock % 10 == 0 ? 128 : 256) + 256
						};
						void updater(s16 j){
							if(bullets[j].clock == 30){
								bullets[j].speed = FIX16(2.25);
								updateBulletVel(j);
							} else if(bullets[j].clock == 60){
								bullets[j].speed = FIX16(2);
								updateBulletVel(j);
							}
						}
						spawnBullet(spawner, updater);
						XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
					}
					// lasers
					if(enemies[i].clock % 15 == 0){
						struct bulletSpawner spawner = {
							.x = enemies[i].pos.x,
							.y = enemies[i].pos.y + FIX16(enemies[i].clock % 30 == 0 ? 48 : -48),
							.image = &hugeBullet,
							.huge = TRUE,
							.speed = FIX16(2),
							.angle = 512 - 32 + random() % 64
						};
						spawnBullet(spawner, EMPTY);
						XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
					}
				}
			} else {
				// homing
				if(enemies[i].clock % 15 == 10){
					struct bulletSpawner spawner = {
						.x = enemies[i].pos.x,
						.y = enemies[i].pos.y,
						.image = &bigBullet,
						.big = TRUE,
						.angle = 256,
						.speed = FIX16(2.5),
						.top = TRUE
					};
					spawner.vel = hone(enemies[i].pos, player.pos, spawner.speed, 16);
					spawnBullet(spawner, EMPTY);
					XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
				}
				// puke
				if(enemies[i].clock % 5 == 3){
					struct bulletSpawner spawner = {
						.x = enemies[i].pos.x,
						.y = enemies[i].pos.y,
						.image = &bigBullet,
						.big = TRUE,
						.angle = 352 + random() % 320,
						.speed = FIX16(2),
						.light = TRUE
					};
					spawner.bools[0] = enemies[i].clock % 5 == 3;
					void updater(s16 j){
						if(bullets[j].clock % 30 == 0){
							bullets[j].angle += bullets[j].bools[0] ? -16 : 16;
							updateBulletVel(j);
						}
					}
					spawnBullet(spawner, updater);
					XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
				}
			}
		} else bossIntro(i);
	}
	void suicide(s16 i){
		stageClock = -60;
		score += 5000;
		bossHealth = 0;
		killBullets = TRUE;
	}
	spawnEnemy(spawner, updater, suicide);
}

static void waveBoss1(){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(1),
		.x = GAME_W + 24,
		.y = GAME_H / 2,
		.image = &flandre,
		.offX = 16,
		.offY = 20,
		.boss = TRUE,
		.health = 80
	};
	bossMax = spawner.health;
	void updater(s16 i){
		if(enemies[i].bools[0]){
			bossHealth = enemies[i].health;
			bossMove(i);
			if(enemies[i].health > 60){
				if(enemies[i].clock % 5 == 0 && enemies[i].clock >= 0){
					struct bulletSpawner spawner = {
						.x = enemies[i].pos.x,
						.y = enemies[i].pos.y,
						.image = &bigBullet,
						.big = TRUE,
						.light = TRUE,
						.speed = FIX16(2.5),
						.top = TRUE,
						.angle = random() % 128 + (enemies[i].clock % 10 == 0 ? 128 : 256) + 256
					};
					spawner.fixes[0] = FIX16(8);
					void updater(s16 j){
						if(bullets[j].pos.x <= bullets[j].fixes[0] && !bullets[j].bools[0]){
							bullets[j].bools[0] = TRUE;
							SPR_setAnim(bullets[j].image, 0);
							bullets[j].angle = random() % 32 - 16;
							bullets[j].speed = FIX16(2);
							updateBulletVel(j);
						}
					}
					spawnBullet(spawner, updater);
					XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
				}
			} else if(enemies[i].health > 40){
				if(!enemies[i].bools[7]){
					SND_startPlayPCM_XGM(SFX_EXPLOSION_2, 15, SOUND_PCM_CH2);
					killBullets = TRUE;
					enemies[i].bools[7] = TRUE;
					enemies[i].clock = -60;
				} else if(!enemies[i].bools[3] && enemies[i].clock >= 0){
					enemies[i].bools[3] = TRUE;
					struct bulletSpawner spawner = {
						.x = enemies[i].pos.x,
						.y = enemies[i].pos.y,
						.image = &hugeBullet,
						.huge = TRUE,
						.angle = 512,
						.speed = FIX16(1.5)
					};
					void updater(s16 j){
						if(bullets[j].clock >= 30 && !bullets[j].bools[0]){
							bullets[j].fixes[0] = bullets[j].pos.x;
							bullets[j].fixes[1] = bullets[j].pos.y;
							bullets[j].bools[0] = TRUE;
							bullets[j].clock = -1;
							bullets[j].angle = 768 - 64;
							updateBulletVel(j);
						} else if(bullets[j].bools[0]){
							if(bullets[j].clock % 10 == 0){
								bullets[j].angle += 64;
								if(bullets[j].angle >= 1024){
									bullets[j].angle = 0;
								}
								if(bullets[j].angle == 768){
									bullets[j].pos.x = bullets[j].fixes[0];
									bullets[j].pos.y = bullets[j].fixes[1];
								}
								updateBulletVel(j);
							}
							if(bullets[j].clock % 15 == 0){
								struct bulletSpawner spawner = {
									.x = bullets[j].pos.x,
									.y = bullets[j].pos.y,
									.image = &bigBullet,
									.big = TRUE,
									.angle = 256 + 128,
									.light = bullets[j].clock % 30 == 0,
									.top = TRUE,
									.speed = FIX16(bullets[j].clock % 30 == 0 ? 2 : 3)
								};
								spawnBullet(spawner, EMPTY);
								for(u8 h = 0; h < 5; h++){
									spawnBullet(spawner, EMPTY);
									spawner.angle += 64;
								}
								XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
							}
						}
					}
					spawnBullet(spawner, updater);
				}
			} else if(enemies[i].health > 20){
				if(!enemies[i].bools[6]){
					SND_startPlayPCM_XGM(SFX_EXPLOSION_2, 15, SOUND_PCM_CH2);
					killBullets = TRUE;
					enemies[i].bools[6] = TRUE;
					enemies[i].clock = -60;
				} else if(enemies[i].clock >= 0) {
					if(enemies[i].clock % 3 == 1){
						struct bulletSpawner spawner = {
							.y = FIX16(random() % (GAME_H - 64) + 32),
							.x = FIX16(GAME_W - 48 + random() % 32),
							.image = &bigBullet,
							.big = TRUE,
							.angle = 512 - 32 + random() % 64,
							.speed = FIX16(enemies[i].clock % 6 == 1 ? 2.5 : 3.5),
							.light = enemies[i].clock % 6 == 1
						};
						spawnBullet(spawner, EMPTY);
						XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
					}
				}
			} else {
				if(!enemies[i].bools[5]){
					SND_startPlayPCM_XGM(SFX_EXPLOSION_2, 15, SOUND_PCM_CH2);
					killBullets = TRUE;
					enemies[i].bools[5] = TRUE;
					enemies[i].clock = -60;
				} else if(enemies[i].clock >= 0) {
					// rays
					if(enemies[i].clock % 240 < 180 && enemies[i].clock % 20 == 0){
						struct bulletSpawner spawner = {
							.x = enemies[i].pos.x,
							.y = enemies[i].pos.y,
							.image = &hugeBullet,
							.huge = TRUE,
							.angle = 120 + random() % 16 + 256,
							.speed = FIX16(3),
						};
						for(u8 j = 0; j < 3; j++){
							spawnBullet(spawner, EMPTY);
							spawner.angle += 128;
						}
						XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
					}
					// homing
					if(enemies[i].clock % 6 == 4){
						struct bulletSpawner spawner = {
							.x = enemies[i].pos.x,
							.y = enemies[i].pos.y,
							.speed = FIX16(2.5),
							.angle = 256,
							.image = &bigBullet,
							.light = TRUE,
							.big = TRUE,
							.top = TRUE
						};
						spawner.vel = hone(enemies[i].pos, player.pos, spawner.speed, 96);
						spawnBullet(spawner, EMPTY);
						XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH4);
					}
				}
			}
		} else bossIntro(i);
	}
	void suicide(s16 i){
		score += 10000;
		bossHealth = 0;
		killBullets = TRUE;
		gameOver = TRUE;
		wonGame = TRUE;
	}
	spawnEnemy(spawner, updater, suicide);
}


// bonus 
s16 currentBonus;
void spawnBonus(bool bottom){
	struct enemySpawner spawner = {
		.angle = 512,
		.speed = FIX16(0.5),
		.x = GAME_W + 8,
		.y = bottom ? GAME_H - 48 : 48,
		.image = &candy,
		.offX = 8,
		.offY = 8,
		.bonus = TRUE,
		.health = 4
	};
	void suicide(s16 i){
		if(enemies[i].pos.x > 0){
			currentBonus += 1000;
			score += currentBonus;
			drawBonus(currentBonus);
		}
	}
	spawnEnemy(spawner, EMPTY, suicide);
}


// loop

s16 currentWave;

void loadStage(){
	stageClock = -180;
	currentWave = 0;
}

void updateStage(){
	if(enemyCount == 0 && stageClock >= 0 && !gameOver){
		switch(currentWave){
			// sine from top
			case 0:
				waveSine(FALSE);
				break;
			// sine from bottom
			case 1:
				waveSine(TRUE);
				break;
			// big middle
			case 2:
				waveBig1();
				break;
			// sine from top, big right
			case 3:
				waveSine(FALSE);
				waveBig2(FALSE);
				spawnBonus(FALSE);
				break;
			// sine from bottom, big left
			case 4:
				waveSine(TRUE);
				waveBig2(TRUE);
				break;
			// big middle
			case 5:
				waveBig3();
				break;
			// swarm (8)
			case 6:
				waveSwarm(8);
				break;
			// big left and right
			case 7:
				waveBig4(TRUE, FALSE);
				waveBig4(FALSE, FALSE);
				break;
			// swarm (2), big right
			case 8:
				waveSwarm(2);
				waveBig4(TRUE, TRUE);
				spawnBonus(FALSE);
				break;
			// swarm (2), big left
			case 9:
				waveSwarm(2);
				waveBig4(FALSE, TRUE);
				break;
			// midboss
			case 10:
				waveMidboss1();
				break;
			// big from top and bottom
			case 11:
				waveBig5(FALSE);
				waveBig5(TRUE);
				break;
			// big from right and left
			case 12:
				waveBig6(TRUE);
				waveBig6(FALSE);
				spawnBonus(TRUE);
				break;
			// big from left, small formation
			case 13:
				waveBig6(FALSE);
				waveFormation(TRUE);
				break;
			// big from right, small formation
			case 14:
				waveBig6(TRUE);
				waveFormation(FALSE);
				break;
			// big from left, small formation
			case 15:
				waveBig6(FALSE);
				waveFormation(TRUE);
				spawnBonus(TRUE);
				break;
			// big from right, small formation
			case 16:
				waveBig6(TRUE);
				waveFormation(FALSE);
				break;
			// midboss
			case 17:
				waveMidboss2();
				break;
			case 18:
				waveSwarm(3);
				spawnBonus(FALSE);
				break;
			// boss
			case 19:
				waveBoss1();
				break;
		}
		currentWave++;
	}

	stageClock++;
	if(stageClock >= CLOCK_LIMIT) stageClock -= CLOCK_LIMIT;
}