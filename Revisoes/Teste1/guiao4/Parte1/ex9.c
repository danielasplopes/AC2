#include <detpic32.h>

void configIO(void);

int main(void){
	static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0X66, 0X6D, 0X7D, 0X07,
										  0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71};
	int value = 0;
	configIO();

	while(1){
		value = PORTB & 0x000F;		// Read dip-switch
		LATB = (LATB & 0x80FF);		// convert to 7 segments code
		LATB |= display7Scodes[value] << 8;	// send to displays	
	}
	return 0;
}

void configIO(void){
	// Configuration of RB0-RB3 as inputs
	TRISB |= 0x000F;
	// Configuration of RB8-RB14 as outputs
	LATB &= 0x80FF;
	TRISB &= 0x80FF;
	// Configuration of RD5-RD6 as outputs
	LATD &= 0xFF9F;
	TRISD &= 0xFF9F;

	LATDbits.LATD5 = 0;
	LATDbits.LATD6 = 1;
}

