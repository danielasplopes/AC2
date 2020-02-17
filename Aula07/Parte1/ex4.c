#include <detpic32.h>

void configTimer(void);
void configInterrupt(void);

unsigned char flag = 0;

int main(void){
	configTimer();
	configInterrupt();

	while(1);

	return 0;
}

void configTimer(void){
	// Configuration of Timer T1
	T1CONbits.TCKPS = 3;
	PR1 = 39062;
	TMR1 = 0;				// Reset timer T2 count register
	T1CONbits.TON = 1;		// Enable timer T2 (Must be the last command of the
							//                  timer configuration sequence!!!)
	// Configuration of Timer T3
	T3CONbits.TCKPS = 5;
	PR3 = 62499;
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

void _int_(_TIMER_1_VECTOR) isr_T1(void){
	putChar('1');
	IFS0bits.T1IF = 0;		// Reset timer T3 interrupt flag
}

void _int_(_TIMER_3_VECTOR) isr_T3(void){
	putChar('3');
	IFS0bits.T3IF = 0;		// Reset timer T3 interrupt flag
}
