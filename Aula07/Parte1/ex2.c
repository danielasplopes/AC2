#include <detpic32.h>

void configTimer(void);
void configInterrupt(void);

int main(void){
	configTimer();
	configInterrupt();

	while(1);

	return 0;
}

void configTimer(void){
	// Configuration of Timer T3
	T3CONbits.TCKPS = 7;
	PR3 = 39062;
	TMR3 = 0;				// Reset timer T2 count register
	T3CONbits.TON = 1;		// Enable timer T2 (Must be the last command of the
							//                  timer configuration sequence!!!)
}

void configInterrupt(void){
	IPC3bits.T3IP = 2;		// Interrupt priority
	IEC0bits.T3IE = 1;		// Enable timer T3 interrupts
	IFS0bits.T3IF = 0;		// Reset timer T3 interrupt flag
	EnableInterrupts();
}

void _int_(_TIMER_3_VECTOR) isr_T3(void){
	putChar('.');
	IFS0bits.T3IF = 0;		// Reset timer T3 interrupt flag
}
