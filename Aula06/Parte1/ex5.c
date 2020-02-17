#include <detpic32.h>

void config_IO(void);
void config_ADC(void);
void configTimer(void);
void configInterrupt(void);

int main(void){
	config_IO();
	config_ADC();
	configTimer();
	configInterrupt();
}

void config_IO(void){
	LATDbits.LATD6 = 1;
	LATDbits.LATD5 = 0;
	// Configure RB8-RB14 as outputs
	LATB &= 0x80FF;
	TRISB &= 0x80FF;
	// Configure RD5-RD6 as outputs
	LATD &= 0xFF9F;
	TRISD &= 0xFF9F;
}

void config_ADC(void){
	TRISBbits.TRISB4 = 1;		// RB4 digital output disconnected
	AD1PCFGbits.PCFG4 = 0;		// RB4 configured as analog input (AN4)

	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;		// Sample time (always 16!)
	AD1CON2bits.SMPI = 7;		// Number os samples (0 to 15) : 0 means 1 sample
								//								 15 means 16 samples
	AD1CHSbits.CH0SA = 4;		// AN4 = RB4 (potenciometro)
	AD1CON1bits.ON = 1;			// ADC on 
}

void configTimer(void){
	// Configuration of Timer T1
	T1CONbits.TCKPS = 3;
	PR1 = 19530;
	TMR1 = 0;				// Reset timer T2 count register
	T1CONbits.TON = 1;		// Enable timer T2 (Must be the last command of the
							//                  timer configuration sequence!!!)
	// Configuration of Timer T3
	T3CONbits.TCKPS = 2;
	PR3 = 49999;
	TMR3 = 0;				// Reset timer T2 count register
	T3CONbits.TON = 1;		// Enable timer T2 (Must be the last command of the
							//                  timer configuration sequence!!!)
}

void configInterrupt(void){
	IPC1bits.T1IP = 2;		// Interrupt priority
	IEC0bits.T1IE = 1;		// Enable timer T3 interrupts
	IFS0bits.T1IF = 0;		// Reset timer T3 interrupt flag

	IPC3bits.T3IP = 2;		// Interrupt priority
	IEC0bits.T3IE = 1;		// Enable timer T3 interrupts
	IFS0bits.T3IF = 0;		// Reset timer T3 interrupt flag

	EnableInterrupts();
}
