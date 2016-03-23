// FakeBrick9 by Wolf, thanks to A_Random_Guy for the idea

#include "draw.h"
#include "memory.h"
#include "gfx.h"

#include "lz.h" // from BCL (Basic Compression Library)

extern u32 rand();
extern void sleep(u32 miliseconds);

struct framebuffers { // thsnks to mid-kid for fb offsets
    u8 *top_left;
    u8 *top_right;
    u8 *bottom;
} *framebuffers = (struct framebuffers *) 0x23FFFE00;

u32 fb_sz(u8* fb) {
	if (fb == framebuffers->top_left || fb == framebuffers->top_right)
		return TOP_FB_SZ;
	else return BOTTOM_FB_SZ;
}

void SetPixel(u8* fb, u32 x, u32 y, u8 b, u8 g, u8 r) {
	u32 offset = (240*x + 240 - y - 1) * 3;
	*((u8*)fb + offset++) = b;
	*((u8*)fb + offset++) = g;
	*((u8*)fb + offset++) = r;
}

void clearScreen() { // clear screens to black
	memset(framebuffers->top_left, 255, TOP_FB_SZ);
	memset(framebuffers->bottom, 255, BOTTOM_FB_SZ);
}

void setScreenColor(u8* fb, u32 width, u32 b, u32 g, u32 r) {
	for (u32 x = 0; x < width; x++)
		for (u32 y = 0; y < 240; y++)
			SetPixel(fb, x, y, b, g, r);
}

void drawTopBrick() { // decompress from top_lz to out and copy to top framebuffer
	u8 out[TOP_FB_SZ];
	LZ_Uncompress( top_lz, out, top_lz_len );
	memcpy(framebuffers->top_left, out, TOP_FB_SZ);
}

void drawBottomBrick() { // decompress from bottom_lz to out and copy to bottom framebuffer
	u8 out[BOTTOM_FB_SZ];
	LZ_Uncompress( bottom_lz, out, bottom_lz_len );
	memcpy(framebuffers->bottom, out, BOTTOM_FB_SZ);
}

void corruptScreen(u8* fb, u32 parts) { // when parts go up, speed goes up and randomness goes down

	for (u32 i = 0; i < fb_sz(fb)/parts; i++)
		memset(&fb[i], rand() % 0xFF, 1); // set random data
	
	for (u32 n = 0; n < parts; n++)
		memcpy(&fb[n * (fb_sz(fb)/parts)], fb, fb_sz(fb)/parts); // copy random data over and over
}

void drawMenuHaxCrash() {
	setScreenColor(framebuffers->bottom, 320, 0, 0, 0);
	sleep(750);
	corruptScreen(framebuffers->top_left, 12);
	setScreenColor(framebuffers->bottom, 320, 255, 255, 255);
	sleep(500);
	setScreenColor(framebuffers->bottom, 320, 0, 255, 0);
	sleep(400);
	setScreenColor(framebuffers->bottom, 320, 0, 255, 255);
	sleep(500);
}

void drawBrick() {
	clearScreen();

	drawTopBrick();
	drawBottomBrick();
	return;
}
