#include <detpic32.h>

void config_IO(void);
void config_ADC(void);
void config_interrupt(void);
void delay(int ms);
unsigned int readADC(void);
void send2Displays(unsigned int value);

volatile unsigned int voltage = 0;		// Global variable

int main(void){
	unsigned int cnt = 0;
	// Configure all (digital I/O, analog input, A/D module, interrupts)
	config_IO();
	config_ADC();
	config_interrupt();

	while(1){
		if(cnt % 25 == 0){		// 250 ms (4 samples/second)
			// Start A/D conversion
			AD1CON1bits.ASAM = 1;
		}
		// Send voltage valiable to displays
		send2Displays(voltage);
		cnt++;
		// wait 10 ms
		delay(10);
	}
	return 0;
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

void config_interrupt(void){
	IPC6bits.AD1IP = 2;			// configure priority of A/D interrupts
	IFS1bits.AD1IF = 0;			// clear A/D interrupt flag
	IEC1bits.AD1IE = 1;			// enables A/D interrupts
	EnableInterrupts();			// Global Interrupt Enable
}

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() < PBCLK/1000);
	}
}

unsigned int readADC(void){

}

void send2Displays(unsigned int value){
	static const char display7codes[] = {0x3F, 0x06, 0x5B, 0x4F, 0X66, 0X6D, 0X7D, 0X07,
										 0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71};
	static char flag = 0;
	// Hexadecimal
	//unsigned char digit_low = value & 0x0F;
	//unsigned char digit_high = value >> 4;

	// Decimal
	unsigned char digit_low = value%10;
	unsigned char digit_high = value/10;

	if(flag){	// send2High
		LATDbits.LATD6 = 1;
		LATDbits.LATD5 = 0;

		LATB = (LATB & 0x80FF) | display7codes[digit_high] << 8;
	}
	else {		// send2Low
		LATDbits.LATD6 = 0;
		LATDbits.LATD5 = 1;

		LATB = (LATB & 0x80FF) | display7codes[digit_low] << 8;
	}

	flag = !flag;
}

// Interrupt Handler
void _int_(27) isr_adc(void){
	int *p = (int*)(&ADC1BUF0);
	int amplitude = 0;
	// Calculate buffer average (8 samples)
	// Calculate voltage amplitude
	int i;
	for(i = 0; i < (AD1CON2bits.SMPI+1); i++){
		amplitude += p[i*4];
	}
	// Convert voltage amplitude to decimal. Assign it to "voltage"
	voltage = ((amplitude/(AD1CON2bits.SMPI+1))*33+511)/1023;
	IFS1bits.AD1IF = 0;			// Reset AD1IF flag
}
