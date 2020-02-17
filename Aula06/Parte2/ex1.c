#include <detpic32.h>

void config_IO(void);
void config_ADC(void);
void config_interrupt(void);

volatile int adc_value;

int main(void){
	// Configure all (digital I/O, analog input, A/D module)
	config_IO();
	config_ADC();
	// Configure interrupt system
	config_interrupt();
	// Start A/D conversion
	AD1CON1bits.ASAM = 1;

	while(1){}			// all activity is done by the ISR
	return 0;
}

void config_IO(void){
	// Configure RE0 as output
	LATE &= 0xFFFE;
	TRISE &= 0xFFFE;
}

void config_ADC(void){
	TRISBbits.TRISB4 = 1;		// RB4 digital output disconnected
	AD1PCFGbits.PCFG4 = 0;		// RB4 configured as analog input (AN4)

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;		// Sample time (always 16!)
	AD1CON2bits.SMPI = 0;		// Number os samples (0 to 15) : 0 means 1 sample
								//								 15 means 16 samples
	AD1CHSbits.CH0SA = 4;		// AN4 = RB4 (potenciometro)
	AD1CON1bits.ON = 1;			// ADC on 
}

void config_interrupt(void){
	IPC6bits.AD1IP = 2;			// configure priority of A/D interrupts
	IFS1bits.AD1IF = 0;			// clear A/D interrupt flag
	IEC1bits.AD1IE = 1;			// enables A/D interrupts
	EnableInterrupts();			// Global Interrupt Enable
}

// Interrupt Handler
void _int_(27) isr_adc(void){
	// Reset RE0
	LATEbits.LATE0 = 0;
	// Read ADC1BUF0 value to "adc_value"
	adc_value = (ADC1BUF0, 16 | 3 << 16);
	// Set RE0 
	LATEbits.LATE0 = 1;
	// Start A/D conversion
	AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0;			// Reset AD1IF flag
}
