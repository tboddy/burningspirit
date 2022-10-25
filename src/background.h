// background

#define BG_I 512

#define BG_W 40
#define BG_H 28


// draw

s16 bgI;
Image* bgImage;

static void drawBg(){
	VDP_fillTileMapRect(BG_A, TILE_ATTR_FULL(PAL2, 1, 0, 0, 32), 0, 0, 32, 28);
	for(u16 x = 0; x <= BG_W; x++) for(u16 y = 0; y < BG_H; y++){
		if(y == 10 && x % 8 == 0) VDP_drawImageEx(BG_B, &bgBooks, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 64 + 16), x, y, 0, DMA);
		else {
			if(y < 8 && x % 8 == 0 && y % 8 == 0) VDP_drawImageEx(BG_B, &bgCeil, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 64 + 16 + 64 + 16), x, y, 0, DMA);
			if(y >= 20 && x % 8 == 0 && y % 8 == 4) VDP_drawImageEx(BG_B, &bgFloor, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I), x, y, 0, DMA);
		}
	}
	for(u16 x = 0; x <= BG_W; x++) for(u16 y = 0; y < BG_H; y++){
		if(y == 10 && x % 2 == 0) VDP_drawImageEx(BG_A, &bgBooksOver, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 64 + 16 + 64), x, y, 0, DMA);
		else {
			if(y == 0 && x % 2 == 0) VDP_drawImageEx(BG_A, &bgCeilOver, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 64 + 16 + 64 + 16 + 64), x, y, 0, DMA);
			else if(y == 20 && x % 2 == 0) VDP_drawImageEx(BG_A, &bgFloorOver, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I + 64), x, y, 0, DMA);
		}
	}
	// there has to be a better way to do this
	VDP_clearTileMapRect(BG_A, 0, 8, 32, 2);
	VDP_clearTileMapRect(BG_A, 0, 18, 32, 2);
}


// update

#define BG_TILES 28

s16 bgPos[BG_TILES];
fix16 bgPosF[BG_TILES];
fix16 bgSpeed;

static void scrollBg(){
	for(u8 y = 0; y < BG_TILES; y++){
		if(y == 0) bgSpeed = FIX16(3);
		if(y > 0 && y <= 10) bgSpeed = fix16Sub(bgSpeed, FIX16(0.25));
		if(y >= 20) bgSpeed = fix16Add(bgSpeed, FIX16(0.25));
		bgPosF[y] = fix16Sub(bgPosF[y], bgSpeed);
		if(bgPosF[y] <= FIX16(-64)) bgPosF[y] = fix16Add(bgPosF[y], FIX16(64));
		bgPos[y] = fix16ToInt(bgPosF[y]);
	}
}


// loop

void loadBg(){
	VDP_setScrollingMode(HSCROLL_TILE, VSCROLL_PLANE);
	drawBg();
}

void updateBg(){
	// VDP_setVerticalScroll(BG_B, bgPos);
	VDP_setHorizontalScrollTile(BG_B, 0, bgPos, BG_TILES, DMA);
	scrollBg();
}