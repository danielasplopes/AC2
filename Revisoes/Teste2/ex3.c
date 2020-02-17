/*
	Regular o PWM consoante o duty-cycle de acordo com ADC
*/

#include <detpic32.h>

void config_IO(void);
void config_ADC(void);
void config_PWM(void);
void configTimer(void);
void setPWM(unsigned int dc);
unsigned int readADC(void);

int main(void){
	unsigned int value = 0;

	config_IO();
	config_ADC();
	configTimer();
	config_PWM();

	while(1){
		AD1CON1bits.ASAM = 1;
		while(!IFS1bits.AD1IF);
		IFS1bits.AD1IF = 0;
		value = readADC();
		setPWM((value*100+511)/1023);
	}

	return 0;
}

void config_IO(void){
	// Configuration of RD0 as output
	LATD &= 0xFFFE;
	TRISD &= 0xFFFE;
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

void configTimer(void){			// 100Hz
	T2CONbits.TCKPS = 2;
	PR2 = 49999;
	TMR2 = 0;
	T2CONbits.TON = 1;
}

void config_PWM(void){
	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 0;
	OC1RS = 12500;
	OC1CONbits.ON = 1;
}

void setPWM(unsigned int dc){
	if(dc > 100) dc = 100;
	OC1RS = (dc*(PR2+1))/100;
}

unsigned int readADC(void){
	int *p = (int*)(&ADC1BUF0);
	int sum = 0;

	int i;
	for(i = 0; i < (AD1CON2bits.SMPI + 1); i++){
		sum += p[i*4];
	}

	return sum/(AD1CON2bits.SMPI + 1);
}
