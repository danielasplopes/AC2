#include <detpic32.h>

void configTimer(void);

int main(void){
	configTimer();

	while(1){
		while(!IFS0bits.T3IF);		// Wait until T3IF = 1
		IFS0bits.T3IF = 0;			// Reset T3IF

		putChar('.');
	}

	return 0;
}

void configTimer(void){
	T3CONbits.TCKPS = 7;
	PR3 = 39062;
	TMR3 = 0;					// Reset timer 3
	T3CONbits.TON = 1;			// Enable timer 3
}
