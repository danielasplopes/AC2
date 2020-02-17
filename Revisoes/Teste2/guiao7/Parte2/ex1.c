#include <detpic32.h>

void config_IO(void);
void configTimer(void);
void config_PWM(void);
void setPWM(unsigned int dc);

int main(void){
	config_IO();
	configTimer();
	config_PWM();

	while(1){
		setPWM(25);
	}

	return 0;
}

void config_IO(void){
	// Configuration of RD0 as output
	TRISD &= 0xFFFE;
}

void configTimer(void){
	// Configuration of timer T3 - 100Hz
	T3CONbits.TCKPS = 2;
	PR3 = 49999;
	TMR3 = 0;
	T3CONbits.TON = 1;
}

void config_PWM(void){
	OC1CONbits.OCM = 6;		// PWM mode on OC1; fault pin disabled
	OC1CONbits.OCTSEL = 1;	// Use timer T3 as the time base for PWM generation
	OC1RS = 12500;			// Ton constant
	OC1CONbits.ON = 1;		// Enable OC1 module
}

void setPWM(unsigned int dc){
	if(dc < 0) dc = 0;
	if(dc > 100) dc = 100;
	OC1RS = ((PR3+1)+dc)/100;
}
