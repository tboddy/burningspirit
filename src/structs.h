// structs


// bullets

#define BULLET_COUNT 64
struct bulletSpawner {
	fix16 x, y, speed;
	Vect2D_f16 vel;
	s16 angle;
	SpriteDefinition* image;
	bool big, player, huge, light, top;
	bool bools[COUNT_INT];
	s16 ints[COUNT_INT];
	fix16 fixes[COUNT_INT];
};
struct bullet {
	bool active, player, dead, big, huge, light;
	fix16 speed;
	fix32 dist, distCheck;
	Vect2D_f16 pos, vel;
	s16 angle, clock;
	Sprite* image;
	void (*updater)(s16);
	bool bools[COUNT_INT];
	s16 ints[COUNT_INT];
	fix16 fixes[COUNT_INT];
};
struct bullet bullets[BULLET_COUNT];


// player

struct playerStruct {
	Vect2D_f16 pos, vel;
	Sprite* image;
	Sprite* hitboxImage;
	s16 clock, invincibleClock, shotClock;
	s16 lives, power;
	bool hit, recovering;
};
struct playerStruct player;


// enemies

#define ENEMY_COUNT 32

struct enemySpawner {
	fix16 speed;
	s16 angle, offX, offY, x, y, anim, health, frame;
	Vect2D_f16 vel;
	Sprite* image;
	bool boss, seal, bonus;
	bool bools[COUNT_INT];
	s16 ints[COUNT_INT];
	fix16 fixes[COUNT_INT];
};

struct enemy {
	bool active, boss, seen, seal, dead, bonus;
	fix16 speed;
	fix32 dist;
	Vect2D_f16 pos, vel, off, last;
	s16 angle, clock, health, flipClock;
	Sprite* image;
	Sprite* sealImage;
	void (*updater)(s16);
	void (*bossUpdater)(s16);
	void (*suicide)(s16);
	bool bools[COUNT_INT];
	s16 ints[COUNT_INT];
	fix16 fixes[COUNT_INT];
};
struct enemy enemies[ENEMY_COUNT];


// powerups

#define POWERUP_COUNT 16
struct powerup {
	bool active;
	u8 type;
	s16 clock;
	Vect2D_f16 pos, vel;
	Sprite* image;
	fix16 speed;
	bool found;
};
struct powerup powerups[POWERUP_COUNT];


// explosion

#define EXPLOSION_COUNT 32

struct explosion {
	bool active;
	s16 clock;
	Sprite* image;
	Vect2D_f16 pos;
	u8 type;
};
struct explosion explosions[EXPLOSION_COUNT];