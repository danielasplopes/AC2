#include <detpic32.h>

void configIO(void);
void config_ADC(void);
unsigned int readADC(void);
void delay(int ms);

int main(void){
	static const char display7codes[] = {0x3F, 0x06, 0x5B, 0x4F, 0X66, 0X6D, 0X7D, 0X07,
										 0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71};
	static char flag = 0;

	unsigned int val = 0;
	unsigned int value = 0;

	configIO();
	config_ADC();

	while(1){
		AD1CON1bits.ASAM = 1;
		while(!IFS1bits.AD1IF);
		IFS1bits.AD1IF = 0;
		val = readADC();
		value = (val*33+511)/1023;

		if(value < 9){
			if(flag){
				LATDbits.LATD5 = 0;
				LATDbits.LATD6 = 1;

				LATB = (LATB & 0x80FF);
				LATBbits.LATB14 = 1;
			}
			else {
				LATDbits.LATD5 = 1;
				LATDbits.LATD6 = 0;

				LATB = (LATB & 0x80FF) | (int)display7codes[(9-value)%10] << 8;
			}
		}
		else {
			if(flag){
				LATDbits.LATD5 = 0;
				LATDbits.LATD6 = 1;

				LATB = (LATB & 0x80FF) | (int)display7codes[(value-9)/10] << 8;
			}
			else {
				LATDbits.LATD5 = 1;
				LATDbits.LATD6 = 0;

				LATB = (LATB & 0x80FF) | (int)display7codes[(value-9)%10] << 8;
			}
		}
		flag ^= 0x01;

		delay(10);
	}

	return 0;
}

void configIO(void){
	LATB &= 0x80FF;
	TRISB &= 0x80FF;
	LATD &= 0xFF9F;
	TRISD &= 0xFF9F;
}

void config_ADC(void){
	TRISBbits.TRISB4 = 1;
	AD1PCFGbits.PCFG4 = 0;

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;		// Sample time (always 16!)
	AD1CON2bits.SMPI = 0;		// Number os samples (0 to 15) : 0 means 1 sample
								//								 15 means 16 samples
	AD1CHSbits.CH0SA = 4;		// AN4 = RB4;
	AD1CON1bits.ON = 1;			// ADC on 
}

unsigned int readADC(void){
	int *p = (int *)(&ADC1BUF0);
	int sum = 0;

	int i;
	for(i = 0; i < (AD1CON2bits.SMPI+1); i++){
		sum += p[i*4];
	}

	return sum/(AD1CON2bits.SMPI+1);
}

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() < PBCLK/1000);
	}
}
