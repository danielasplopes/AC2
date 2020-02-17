#include <detpic32.h>

void delay(int ms);

void main(void){
	unsigned char segment;
	LATDbits.LATD6 = 1;		// display high active
	LATDbits.LATD5 = 0;		// display low inactive

	// configure RB8-RB14 as outputs
	LATB = LATB && 0x80FF;
	TRISB = TRISB && 0x80FF;
	// configure RD5-RD6 as outputs
	LATD = LATD && 0xFF9F;
	TRISD = TRISD && 0xFF9F;

	while(1){
		LATDbits.LATD6 = !LATDbits.LATD6;
		LATDbits.LATD5 = !LATDbits.LATD5;	// toggle display selection
		segment = 1;
		int i;
		for(i = 0; i < 7; i++){
			// send "segment" value to display
			LATB = segment << 8;
			// wait 0.5 second
			delay(500);
			segment = segment << 1;
		}
	}
}

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() < PBCLK/1000);	// PBCLK = 20 000 000 logo 
												// PBCLK/1000 para dar 20 000
	}
}

