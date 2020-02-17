#include <detpic32.h>

void config_IO(void);
void config_ADC(void);
void configTimer(void);
void config_PWM(void);
void config_UART(void);
void config_interrupt(void);
void setPWM(unsigned int dc);
void putc(char byte2send);
void putS(char *str);
char getc(void);
unsigned int readADC(void);
void send2Displays(unsigned int value);

volatile unsigned int voltage = 0;		// Global variable
volatile int voltMin = 33;
volatile int voltMax = 0;

int main(void){
	unsigned int dutyCycle = 0;
	// Configure all (digital I/O, analog input, A/D module, interrupts)
	config_IO();
	config_ADC();
	configTimer();
	config_PWM();
	config_UART();
	config_interrupt();

	while(1){
		switch(PORTB & 0x0003){		// Reads the 2 lsb from switches
			case 0:	// Mesure input voltage
				IEC0bits.T1IE = 1;		// Enable T1 interrupts
				setPWM(0);				// PWM off
				break;
			case 1:	// Freeze
				IEC0bits.T1IE = 0;		// Disable T1 interrupts
				setPWM(100);			// PWM on - dc = 100
				break;
			default:	// LED brightness control
				IEC0bits.T1IE = 1;		// Enable T1 interrupts
				dutyCycle = voltage*3;
				putS("dutyCycle: ");
				printInt10(dutyCycle);
				setPWM(dutyCycle);
				break;
		}
	}

	return 0;
}

void config_IO(void){
	LATDbits.LATD6 = 1;
	LATDbits.LATD5 = 0;
	// Configuration of RB8-RB14 as outputs
	LATB &= 0x80FF;
	TRISB &= 0x80FF;
	// Configuration of RD5-RD6 and RD0 as outputs
	LATD &= 0xFF9E;
	TRISD &= 0xFF9E;
	// Configurtion of RB0-RB1 as inputs
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

void configTimer(void){
	//Configuration of timer T1 - 4Hz
	T1CONbits.TCKPS = 3;
	PR1 = 19530;
	TMR1 = 0;
	T1CONbits.TON = 1;
	// Configuration of timer T3 - 100Hz
	T3CONbits.TCKPS = 2;
	PR3 = 49999;
	TMR3 = 0;
	T3CONbits.TON = 1;
}

void config_PWM(void){
	OC1CONbits.OCM = 6;		// PWM mode on OC1; fault pin disabled
	OC1CONbits.OCTSEL = 1;	// Use timer T3 as the time base for PWM generation
	OC1RS = 0;			// Ton constant
	OC1CONbits.ON = 1;		// Enable OC1 module
}

void config_UART(void){
	int baud = 115200;
	U1MODEbits.BRGH = 0;
	U1BRG = ((PBCLK+8*baud)/(16*baud))-1;
	U1MODEbits.PDSEL = 00;
	U1MODEbits.STSEL = 0;
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	U1MODEbits.ON = 1;
}

void config_interrupt(void){
	// ADC interruptions
	IPC6bits.AD1IP = 2;			// configure priority of A/D interrupts
	IFS1bits.AD1IF = 0;			// clear A/D interrupt flag
	IEC1bits.AD1IE = 1;			// enables A/D interrupts
	// Timer T1 interruptions
	IPC1bits.T1IP = 2;
	IEC0bits.T1IE = 1;
	IFS0bits.T1IF = 0;
	// Timer T3 interruptions
	IPC3bits.T3IP = 2;
	IEC0bits.T3IE = 1;
	IFS0bits.T3IF = 0;
	/* UART interrupts */
	IPC6bits.U1IP = 2;					// Priority
	IEC0bits.U1RXIE = 1;				// Receive Interrupt Enable
	IEC0bits.U1TXIE = 0;				// Transmit Interrupt Enable
	IEC0bits.U1EIE = 1;					// Fazer deteção de erros por interrupção

	IFS0bits.U1RXIF = 0;
	IFS0bits.U1TXIF = 0;
	IFS0bits.U1EIF = 0;

	EnableInterrupts();			// Global Interrupt Enable
}

void setPWM(unsigned int dc){
	if(dc < 0) dc = 0;
	if(dc > 100) dc = 100;
	OC1RS = ((PR3+1)*dc)/100;
}

void putc(char byte2send){
	while(U1STAbits.UTXBF);
	U1TXREG = byte2send;
}

void putS(char *str){
	while(*str != '\0'){
		putc(*str);
		str++;
	}
}

char getc(void){
	if(U1STAbits.OERR)
		U1STAbits.OERR = 0;
	while(U1STAbits.URXDA == 0);
	return U1RXREG;
}

unsigned int readADC(void){
	int *p = (int*)(&ADC1BUF0);
	int amplitude = 0;
	// Calculate buffer average (8 samples)
	// Calculate voltage amplitude
	int i;
	for(i = 0; i < (AD1CON2bits.SMPI+1); i++){
		amplitude += p[i*4];
	}
	// Convert voltage amplitude to decimal. Assign it to "voltage"
	return (amplitude/(AD1CON2bits.SMPI+1));
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

// Interrupt ADC
void _int_(27) isr_adc(void){
	voltage = (readADC()*33+511)/1023;

	if(voltMin > voltage) voltMin = voltage;
	if(voltMax < voltage) voltMax = voltage;

	IFS1bits.AD1IF = 0;			// Reset AD1IF flag
}

//Interruption timer T1
void _int_(_TIMER_1_VECTOR) isr_T1(void){
	AD1CON1bits.ASAM = 1;		// Start conversion
	IFS0bits.T1IF = 0;			// Reset T1IF flag
}

// Interruption timer T3
void _int_(_TIMER_3_VECTOR) isr_T3(void){
	static int counter = 0;
	send2Displays(voltage);			// send voltage to displays
	if(++counter == 100){
		counter = 0;
		putc(0x30 + voltage/10);
		putc(0x30 + voltage%10);
		putc('\n');
	}
	IFS0bits.T3IF = 0;
}

void _int_(24) isr_UART1(void){
	if(U1RXREG == 'L'){
		// Send voltMin and voltMax to serial port UART1
		putS("VoltMin = ");
		putc(0x30 + voltMin/10);
		putc(0x30 + voltMin%10);
		putc('\n');

		putS("VoltMax = ");
		putc(0x30 + voltMax/10);
		putc(0x30 + voltMax%10);
		putc('\n');
	}
	IFS0bits.U1RXIF = 0;
}
