#include <detpic32.h>

void configIO(void);
void delay(int ms);

int main(void){
	unsigned char segment;

	configIO();

	while(1){
		LATDbits.LATD6 = !LATDbits.LATD6;
		LATDbits.LATD5 = !LATDbits.LATD5;
		segment = 1;

		int i;
		for(i = 0; i < 7; i++){
			LATB = segment << 8;
			delay(500);			// wait 0.5 seg -> 2 Hz
			segment = segment << 1;
		}
	}
	return 0;
}

void configIO(void){
	// Configuration of RD5-RD6 as outputs
	LATD &= 0xFF9F;
	TRISD &= 0xFF9F;
	// Configuration of RB8-RB14 as outputs
	LATB &= 0x80FF;
	TRISB &= 0x80FF;

	LATDbits.LATD5 = 0;
	LATDbits.LATD6 = 1;
}

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() < PBCLK/1000);
	}
}
