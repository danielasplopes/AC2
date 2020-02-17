#include <detpic32.h>

void configIO(void);
void delay(int ms);
void send2Displays(unsigned char value);
unsigned char toBcd(unsigned char value);

int main(void){
	unsigned char count = 0;
	unsigned int i = 0;
	configIO();

	while(1){
		i = 0;
		do{
			send2Displays(toBcd(count));
			delay(10);				// wait 10 ms
		} while(++i < 100);			// 10*100 = 1000ms = 1Hz
		count = (count+1)%60;		// increment counter (module 60)
	}

	return 0;
}

void configIO(void){
	// Configuration of RB8-RB14 as outputs
	LATB &= 0x80FF;
	TRISB = (TRISB & 0x80F0) | 0x000F;
	// Configuration of RD5-RD6 as outputs
	LATD &= 0xFF9F;
	TRISD &= 0xFF9F;

	LATDbits.LATD5 = 0;
	LATDbits.LATD6 = 1;
}

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() < PBCLK/1000);
	}
}

void send2Displays(unsigned char value){
	static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0X66, 0X6D, 0X7D, 0X07,
										  0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71};

	static char flag = 0;

    unsigned char digit_low = value & 0x0F;
    unsigned char digit_high = value >> 4;

    if(flag){	// send 2 high
    	LATDbits.LATD5 = 0;
    	LATDbits.LATD6 = 1;

    	LATB = (LATB & 0x80FF) | (int)display7Scodes[digit_high] << 8;
    }
    else{	// send 2 low
    	LATDbits.LATD5 = 1;
    	LATDbits.LATD6 = 0;

    	LATB = (LATB & 0x80FF) | (int)display7Scodes[digit_low] << 8;
    }
    flag = !flag;
}

unsigned char toBcd(unsigned char value){
	return ((value/10)<<4) + (value%10);
}
