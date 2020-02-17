#include <detpic32.h>

void delay(int ms);
void send2Displays(unsigned char value);

void main(void){
	unsigned char counter = 0;
	LATD = (LATD & 0xFF9F) | 0x0040;		// display high active, low inactive

	// configure RB8-RB14 as outputs
	LATB = LATB && 0x80FF;
	TRISB = TRISB && 0x80FF;
	// configure RD5-RD6 as outputs
	LATD = LATD && 0xFF9F;
	TRISD = TRISD && 0xFF9F;

	while(1){
		int i = 0;
		do{
			delay(5);
			send2Displays((counter));
		} while(++i < 100);
		counter = (counter+1);
	}
}

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() < PBCLK/1000);	// PBCLK = 20 000 000 logo 
												// PBCLK/1000 para dar 20 000
	}
}


void send2Displays(unsigned char value){
	static const char display7codes[] = {0x3F, 0x06, 0x5B, 0x4F, 0X66, 0X6D, 0X7D, 0X07,
										 0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71};
	static char flag = 0;

	unsigned char digit_low = value & 0x0F;
	unsigned char digit_high = value >> 4;

	if(flag){	// send2High
		LATDbits.LATD6 = 1;
		LATDbits.LATD5 = 0;

		LATB = (LATB & 0x80FF) | display7codes[digit_high] << 8;
	}
	else {		// send2Low
		LATDbits.LATD6 = 0;
		LATDbits.LATD5 = 1;

		LATB = (LATB & 0x80FF) | display7codes[digit_low] << 8;
	}

	flag = !flag;
}
