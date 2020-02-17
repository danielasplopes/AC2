#include <detpic32.h>

void delay(int ms);
void configIO(void);

int main(void){
	configIO();
	int count = 0;
	while(1){
		LATE = (LATE & 0xFFF0) | count++;
		delay(250);
	}
	return 0;
}

void configIO(void){
	// Configure RE0-RE3 as outputs
	LATE &= 0xFFF0;
	TRISE &= 0xFFF0;
}

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() < PBCLK/1000);
	}
}
