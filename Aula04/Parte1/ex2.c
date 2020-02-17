#include <detpic32.h>

void delay(int ms);

void main(void){
	LATE = LATE & 0xFFF0;			// The initial value should be set before
									// configuring the port as output
	TRISE = (TRISE & 0xFFF0); 		// RE0 a RE3 = 0 (0 means OUTPUT)

	int count = 0;
	while(1){
		delay(250);					// atualiza a uma freq de 4 Hz
		count++;
		LATE = (LATE & 0xFFF0) | count;		// envia para os leds a contagem do count
	}
}

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() < PBCLK/1000);	// PBCLK = 20 000 000 logo 
												// PBCLK/1000 para dar 20 000
	}
}
