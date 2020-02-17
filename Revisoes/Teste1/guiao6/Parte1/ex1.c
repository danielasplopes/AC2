#include <detpic32.h>

void config_ADC(void);
void config_Interrupts(void);
unsigned int readADC(unsigned int value);

int main(void){
	config_ADC();
	config_Interrupts();

	AD1CON1bits.ASAM = 1;		// Start conversion

	while(1){}

	return 0;
}

void config_ADC(void){
	TRISBbits.TRISB4 = 1;		// RB4 digital output disconnected
	AD1PCFGbits.PCFG4 = 0;		// RB4 configured as analog input

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = 0;		// Number of samples -> x-1 (x is the number of samples)
	AD1CHSbits.CH0SA = 4;
	AD1CON1bits.ON = 1;
}

void config_Interrupts(void){
	IPC6bits.AD1IP = 2;			// configure priority of A/D interrupts
	IFS1bits.AD1IF = 0;			// clear A/D interupt flag
	IEC1bits.AD1IE = 1;			// Enable A/D interrupts
	EnableInterrupts();
}

unsigned int readADC(unsigned int value){
	int *p = (int *)(&ADC1BUF0);
	int sum = 0;

	int i;
	for(i = 0; i < (AD1CON2bits.SMPI+1); i++){
		sum += p[i*4];
	}

	return sum/(AD1CON2bits.SMPI+1);
}

void _int_(27) isr_adc(void){
	printInt(ADC1BUF0, 16 | 3 << 16);
	printf("\n");
	AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0;
}
