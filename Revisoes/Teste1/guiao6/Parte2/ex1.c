#include <detpic32.h>

void configIO(void);
void config_ADC(void);
void config_Interrupts(void);
void delay(int ms);
unsigned int readADC(void);
void send2Displays(unsigned char value);
unsigned char toBcd(unsigned char value);

volatile unsigned char voltage = 0;

int main(void){
	unsigned int cnt = 0;

	configIO();
	config_ADC();
	config_Interrupts();

	while(1){
		if(cnt % 25 == 0){
			AD1CON1bits.ASAM = 1;		// Start conversion
		} 
		send2Displays(toBcd(voltage));
		cnt++;
		delay(10);						// wait 10 ms
	}

	return 0;
}

void configIO(void){
	// Configuration of RB8-RB14 as outputs
	LATB &= 0x80FF;
	TRISB &= 0x80FF;
	// Configuration of RD5-RD& as outputs
	LATD &= 0xFF9F;
	TRISD &= 0xFF9F;
	// Configuration of RE0 as output
	LATE &= 0xFFFE;
	TRISE &= 0xFFFE;
}

void config_ADC(void){
	TRISBbits.TRISB4 = 1;		// RB4 digital output disconnected
	AD1PCFGbits.PCFG4 = 0;		// RB4 configured as analog input

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = 7;		// Number of samples -> x-1 (x is the number of samples)
	AD1CHSbits.CH0SA = 4;
	AD1CON1bits.ON = 1;
}

void config_Interrupts(void){
	IPC6bits.AD1IP = 2;			// configure priority of A/D interrupts
	IFS1bits.AD1IF = 0;			// clear A/D interupt flag
	IEC1bits.AD1IE = 1;			// Enable A/D interrupts
	EnableInterrupts();
}

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() < PBCLK/1000);
	}
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

void send2Displays(unsigned char value){
	static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0X66, 0X6D, 0X7D, 0X07,
										  0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71};

	static char flag = 0;

	unsigned int digit_low = value & 0x0F;
	unsigned int digit_high = value >> 4;

	if(flag){
		LATDbits.LATD5 = 0;
		LATDbits.LATD6 = 1;

		LATB = (LATB & 0x80FF) | (int)display7Scodes[digit_high] << 8;
	}
	else {
		LATDbits.LATD5 = 1;
		LATDbits.LATD6 = 0;

		LATB = (LATB & 0x80FF) | (int)display7Scodes[digit_low] << 8;
	}

	flag = !flag;
}

unsigned char toBcd(unsigned char value){
	return ((value/10)<<4) + (value%10);
}

void _int_(27) isr_adc(void){
	LATEbits.LATE0 = 0;
	voltage = (readADC()*33+511)/1023;
	LATEbits.LATE0 = 1;
	AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0;
}
