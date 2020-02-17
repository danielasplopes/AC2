#include <detpic32.h>

void config_ADC(void);

int main(void){
	config_ADC();

	while(1){
		AD1CON1bits.ASAM = 1;				// Start conversion
		while(!IFS1bits.AD1IF);			// Wait while conversion not done
		printInt(ADC1BUF0, 16 | 3 << 16);	// Read conversion result
		IFS1bits.AD1IF = 0;				// Reset AD1IF
	}
}

void config_ADC(void){
	TRISBbits.TRISB4 = 1;		// RB4 digital output disconnected
	AD1PCFGbits.PCFG4 = 0;		// RB4 configured as analog input

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = 0;		// Number of samples -> x-1
	AD1CHSbits.CH0SA = 4;
	AD1CON1bits.ON = 1;
}
