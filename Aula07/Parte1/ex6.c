#include <detpic32.h>

void config_IO(void);
void config_ADC(void);
void config_interrupt(void);
void configTimer(void);
void delay(int ms);
void send2Displays(unsigned int value);

volatile unsigned int voltage = 0;		// Global variable

int main(void){
	// Configure all (digital I/O, analog input, A/D module, interrupts)
	config_IO();
	config_ADC();
	config_interrupt();
	configTimer();

	while(1){
		if(PORTBbits.RB1 && !PORTBbits.RB0)	// if RB1 = 0 e RB0 = 1
			IEC0bits.T1IE = 0;				// Disable timer T1 (freeze)
		else IEC0bits.T1IE = 1;				// Enable timer T1
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
	// Configure RB0-RB1 as inputs
	TRISB |= 0x0003;
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

	IPC1bits.T1IP = 2;		// Interrupt priority
	IEC0bits.T1IE = 1;		// Enable timer T3 interrupts
	IFS0bits.T1IF = 0;		// Reset timer T3 interrupt flag

	IPC3bits.T3IP = 2;		// Interrupt priority
	IEC0bits.T3IE = 1;		// Enable timer T3 interrupts
	IFS0bits.T3IF = 0;		// Reset timer T3 interrupt flag

	EnableInterrupts();			// Global Interrupt Enable
}

void configTimer(void){
	// Configuration of Timer T1
	T1CONbits.TCKPS = 3;
	PR1 = 19530;
	TMR1 = 0;				// Reset timer T2 count register
	T1CONbits.TON = 1;		// Enable timer T2 (Must be the last command of the
							//                  timer configuration sequence!!!)
	// Configuration of Timer T3
	T3CONbits.TCKPS = 2;
	PR3 = 49999;
	TMR3 = 0;				// Reset timer T2 count register
	T3CONbits.TON = 1;		// Enable timer T2 (Must be the last command of the
							//                  timer configuration sequence!!!)
}

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() < PBCLK/1000);
	}
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

void _int_(_TIMER_1_VECTOR) isr_T1(void){
	AD1CON1bits.ASAM = 1;	// Start A/D conversion
	IFS0bits.T1IF = 0;		// Reset timer T3 interrupt flag
}

void _int_(_TIMER_3_VECTOR) isr_T3(void){
	send2Displays(voltage);
	IFS0bits.T3IF = 0;		// Reset timer T3 interrupt flag
}
