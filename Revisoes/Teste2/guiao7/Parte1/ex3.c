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
	T3CONbits.TCKPS = 7;
	PR3 = 39062;
	TMR3 = 0;					// Reset timer 3
	T3CONbits.TON = 1;			// Enable timer 3
}

void configInterrupts(void){
	IPC3bits.T3IP = 2;			// Interrupt priority
	IEC0bits.T3IE = 1;			// Enable timer T3
	IFS0bits.T2IF = 0;			// Reset Timer T3

	EnableInterrupts();
}

void _int_(_TIMER_3_VECTOR) isr_T3(void){
	if(flag)
		putChar('.');
	flag ^= 1;
	IFS0bits.T3IF = 0;
}
