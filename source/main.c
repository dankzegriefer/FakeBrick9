// FakeBrick9 by Wolf, thanks to A_Random_Guy for the idea

#include "types.h" // include types
#include "draw.h" // include drawing functions
#include "memory.h" // include basic memory functions

#include "chainloader.h" // include chainloader binary

#define PAYLOAD_ADDRESS 0x24F00000 // taken from bootctr9's bootloaderloader (quite a name, huh?)

#define REG_HID ((volatile u32*)0x10146000) // apparently this is the HID register (thanks b1l1s!)
#define REG_PRNG ((volatile u32*)0x10011000) // this is the PRNG register (thanks 3dbrew.org!) don't really know if it's a u32 but whatever

#define SLEEP_1000 0x5600 // need new measurements, this is WILDLY inaccurate

#define NO_OP "andeq r0, r0, r0" // thanks wikipedia, now I know

u32 rand() {
	return (*REG_PRNG);
}

void sleep(u32 miliseconds) { // please note: NOT PRECISE AT ALL, BARELY WORKS AS INTENDED
	u32 i = (SLEEP_1000*miliseconds); while(--i) __asm(NO_OP); // no_op for SLEEP_1000*miliseconds, no idea if this will perform differently on N3DS (doubt it)
}

int checkKeys() {
	if (*REG_HID != (u32)0xFFF)
		return 1;
	else return 0;
}

void runIntegratedPayload(void){
	memcpy((u8 *)PAYLOAD_ADDRESS, chainloader_bin, chainloader_bin_len); // copy the chainloader to PAYLOAD_ADDRESS
	((void(*)(void))PAYLOAD_ADDRESS)(); // run whatever is at PAYLOAD_ADDRESS, gotta check later for argument loading
}

int main(){
	drawBrick(); // draw brick
	sleep(5000);
	u32 rnd = rand() % 20; // get random number between 0 and 19
	if (checkKeys() == 1 && rnd < 0)
	{
		drawMenuHaxCrash(); // emulate menuhax
		sleep(500); // wait for a while
	}
	runIntegratedPayload(); // run chainloader payload, should load /NANDUnBricker.bin
	return 0;
}
