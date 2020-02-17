#include <detpic32.h>

void configIO(void);
void configTimer(void);
void configInterrupts(void);
void configOC1(void);
void setPWM(unsigned int dutyCycle);

int main(void){
	configIO();
	configTimer();
	configInterrupts();
	configOC1();

	while(1){
		setPWM(25);
	}

	return 0;
}

void configIO(void){
	// Configuration of RD0 as output
	LATD &= 0xFFFE;
	TRISD &= 0xFFFE;
}

void configTimer(void){
	// Configuration of Timer T3
	T3CONbits.TCKPS = 2;
	PR3 = 49999;
	TMR3 = 0;				// Reset timer T2 count register
	T3CONbits.TON = 1;		// Enable timer T2 (Must be the last command of the
							//                  timer configuration sequence!!!)
}

void configInterrupts(void){
	IPC3bits.T3IP = 2;		// Interrupt priority
	IEC0bits.T3IE = 1;		// Enable timer T2 interrupts
	IFS0bits.T3IF = 0;		// Reset timer T2 interrupt flag

	EnableInterrupts();			// Global Interrupt Enable
}

void configOC1(void){
	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 0;
	OC1RS = 12500;
	OC1CONbits.ON = 1;
}

void setPWM(unsigned int dutyCycle){
	if(dutyCycle > 100) dutyCycle = 100;	// duty-cycle must be in the range [0, 100]
	OC1RS = (dutyCycle*(PR2+1))/100; 		// Evaluate OC1RS as function of "dutyCycle"
}

void _int_(_TIMER_3_VECTOR) isr_T3(void){
	IFS0bits.T3IF = 0;
}
