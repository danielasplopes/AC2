#include <detpic32.h>

void send2Displays(unsigned char value){
	static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0X66, 0X6D, 0X7D, 0X07,
										  0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71};

	static char flag = 0;

	unsigned char digit_low = value & 0x0F;
	unsigned char digit_high = value >> 4;

	if(flag){
		LATDbits.LATD5 = 0;
		LATDbits.LATD6 = 1;

		LATD = (LATD & 0x80FF) | (int)display7Scodes[digit_high] << 8;
	}
	else {
		LATDbits.LATD5 = 1;
		LATDbits.LATD6 = 0;

		LATD = (LATD & 0x80FF) | (int)display7Scodes[digit_low] << 8;
	}

	flag = !flag;
}
