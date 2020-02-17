#include <detpic32.h>

void config_ADC(void);
void readADC(void);

int main(void){
	config_ADC();

	while(1){
		AD1CON1bits.ASAM = 1;				// Start conversion
		while(!IFS1bits.AD1IF);			// Wait while conversion not done
		readADC();
		IFS1bits.AD1IF = 0;				// Reset AD1IF
	}
}

void config_ADC(void){
	TRISBbits.TRISB4 = 1;		// RB4 digital output disconnected
	AD1PCFGbits.PCFG4 = 0;		// RB4 configured as analog input

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = 3;		// Number of samples -> x-1
	AD1CHSbits.CH0SA = 4;
	AD1CON1bits.ON = 1;
}

void readADC(void){
	int *p = (int *)(&ADC1BUF0);

	int i;
	for(i = 0; i < (AD1CON2bits.SMPI+1); i++){
		printInt(p[i*4], 16 | 4 << 16);
		putChar(' ');
	}
	printf("\n");
}
