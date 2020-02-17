#include <detpic32.h>

void adc_config(void);
void readADC(void);

int main(void){
	adc_config();
	while(1){
		AD1CON1bits.ASAM = 1;				// Start convertion
		while(!IFS1bits.AD1IF);				// Wait while convertion not done
		readADC();							// Read convertion result
		IFS1bits.AD1IF = 0;					// Reset AD1IF
	}
	return 0;
}

void adc_config(void){
	TRISBbits.TRISB4 = 1;		// RB4 digital output disconnected
	AD1PCFGbits.PCFG4 = 0;		// RB4 configured as analog input

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = 0;		// 1 sample
	AD1CHSbits.CH0SA = 4;		// AN4 = RB4 (analog channel)
	AD1CON1bits.ON = 1;
}

void readADC(void){
	int *p = (int*)(&ADC1BUF0);

	int i;
	for(i = 0; i < 16; i++){
		printInt(p[i*4], 16 |4 << 16);
		putChar(' ');
	}
	putChar('\n');
}
