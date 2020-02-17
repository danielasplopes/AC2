#include <detpic32.h>

void adc_config(void);
unsigned int readADC(void);

unsigned int  value = 0;

int main(void){
	unsigned int v10;
	adc_config();
	while(1){
		AD1CON1bits.ASAM = 1;				// Start convertion
		while(!IFS1bits.AD1IF);				// Wait while convertion not done
		IFS1bits.AD1IF = 0;					// Reset AD1IF
		value = readADC();					// Read convertion result
		v10 = (value*33)/1023;	
		printInt10(v10);
		printf("\n");
	}
	return 0;
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
	
	return soma/(AD1CON2bits.SMPI+1);
}
