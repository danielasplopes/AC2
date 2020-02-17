#include <detpic32.h>

void configIO(void);
void adc_config(void);
unsigned int readADC(void);
void send2Displays(unsigned char value);
void delay(int ms);

unsigned int value = 0;

int main(void){
	unsigned char v10 = 0;
	configIO();
	adc_config();

	int i = 0;
	while(1){
		if((i++) % 25 == 0){
			AD1CON1bits.ASAM = 1;				// Start convertion
			while(!IFS1bits.AD1IF);				// Wait while convertion not done
			IFS1bits.AD1IF = 0;					// Reset AD1IF
			value = readADC();					// Read convertion result
			printInt(value, 16 | 4 << 16);
			printf("  ");
			v10 = (value*33)/1023;
			printInt10(v10);
			printf("\n");	
		}
		send2Displays(v10);
		delay(10);
	}
	return 0;
}

void configIO(void){
	LATDbits.LATD6 = 1;		
	LATDbits.LATD5 = 0;
	// configure RB8-RB14 as outputs
	LATB &= 0x80FF;
	TRISB &= 0x80FF;
	// configure RD5-RD6 as outputs
	LATD &= 0xFF9F;
	TRISD &= 0xFF9F;
}

void adc_config(void){
	TRISBbits.TRISB4 = 1;		// RB4 digital output disconnected
	AD1PCFGbits.PCFG4 = 0;		// RB4 configured as analog input

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = 3;		// 4 samples
	AD1CHSbits.CH0SA = 4;		// AN4 = RB4 (analog channel)
	AD1CON1bits.ON = 1;
}

unsigned int readADC(void){
	int *p = (int*)(&ADC1BUF0);
	int soma = 0;

	int i;
	for(i = 0; i < (AD1CON2bits.SMPI+1); i++){
		soma += p[i*4];
	}
	
	return (soma/(AD1CON2bits.SMPI+1));
}

void send2Displays(unsigned char value){
	static const char display7codes[] = {0x3F, 0x06, 0x5B, 0x4F, 0X66, 0X6D, 0X7D, 0X07,
										 0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71};
	static char flag = 0;
	// Hexadecimal
	//unsigned char digit_low = value & 0x0F;
	//unsigned char digit_high = value >> 4;
	// Decimal
	unsigned char digit_low = value%10;
	unsigned char digit_high = value/10;

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

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() < PBCLK/1000);	// PBCLK = 20 000 000 logo 
												// PBCLK/1000 para dar 20 000
	}
}
