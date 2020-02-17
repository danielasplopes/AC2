#include <detpic32.h>

void configTimer(void);
void configInterrupts(void);

unsigned char flag = 0;

int main(void){
	configTimer();
	configInterrupts();

	while(1);

	return 0;
}

void configTimer(void){
	// Configuration of Timer T1 - 2Hz
	T1CONbits.TCKPS = 3;
	PR1 = 39062;
	TMR1 = 0;					// Reset timer 1
	T1CONbits.TON = 1;			// Enable timer 1
	// Configuration of Timer T3 - 10 Hz
	T3CONbits.TCKPS = 5;
	PR3 = 62499;
	TMR3 = 0;
	T3CONbits.TON = 1;
}

void configInterrupts(void){
	// Interruptions of Timer T1
	IPC1bits.T1IP = 2;			// Interrupt priority
	IEC0bits.T1IE = 1;			// Enable timer T3
	IFS0bits.T1IF = 0;			// Reset Timer T3
	// Interruptions of Timer T3
	IPC3bits.T3IP = 2;			// Interrupt priority
	IEC0bits.T3IE = 1;			// Enable timer T3
	IFS0bits.T3IF = 0;			// Reset Timer T3

	EnableInterrupts();
}

void _int_(_TIMER_1_VECTOR) isr_T1(void){
	putChar('1');
	IFS0bits.T1IF = 0;
}

void _int_(_TIMER_3_VECTOR) isr_T3(void){
	putChar('3');
	IFS0bits.T3IF = 0;
}
