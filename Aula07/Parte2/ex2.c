#include <detpic32.h>

int main(void){
	
}

void setPWM(unsigned int dutyCycle){
	if(dutyCycle > 100) dutyCycle = 100;	// duty-cycle must be in the range [0, 100]
	OC1RS = (dutyCycle*(PR2+1))/100; 		// Evaluate OC1RS as function of "dutyCycle"
}