/*
	Entradas:
		caso 0 -> Display = Tensão ADC (2Hz), escrever nos LEDs de saída um gráfico de barras da tensão lida, PWM Off
		caso 1 -> Display = Tensão ADC (10Hz), PWM On
		caso 2 -> Display = Tensão ADC (2Hz), enviar para monitor sempre que um novo valor for lido, PWM On
		caso 3 -> Display = Congelado com o último valor lido a "piscar" , PWM Off

	Default:
		Imprimir no monitor "valor não válido"

	Se premido letra 'h' -> valor a apresentar no Display em hexadecimal (0-0xFF)
	Se premido letra 'd' -> valor a apresentar no Display em decimal (0-33)

	NÂO PODE usar as funçṍes readCoreTimer() e resetCoreTimer()
*/

#include <detpic32.h>

void config_IO(void);
void config_ADC(void);
void configTimer(void);
void configUART(void);
void configOC1(void);
void setPWM(unsigned char dc);
void putc(char c);
void putS(char *str);
char getc(void);
char inKey(void);
unsigned int readADC(void);
void send2Displays(unsigned int value);
unsigned int toBcd(unsigned int value);
void delay(void);

unsigned int baud = 115200;

int main(void){

	const char HexArray[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
							   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	unsigned int count = 0, value = 0, val = 0;
	unsigned char l, mode = 0;

	config_IO();
	config_ADC();
	configTimer();
	configUART();
	configOC1();

	while(1){
		switch(PORTB & 0x0F){
			case 0: 
			putS("Case0\n\r");
				for(count = 0; count < (100/2); count++){
					delay();

					l = inKey();
					if(l == 'h') mode = 1;
					if(l == 'd') mode = 0;

					if(mode == 1) send2Displays(val);
					else send2Displays(toBcd(val));
				}

				AD1CON1bits.ASAM = 1;
				while(!IFS1bits.AD1IF);
				IFS1bits.AD1IF = 0;
				value = readADC();
				if(mode == 1)
					val = (value*0xFF+511)/ 1023;
				else 
					val = (value*33+511)/ 1023;

				setPWM(0);			// PWM off

				LATE &= 0xFFF0;
				LATE |= (1 << ((value >> 8)+1))-1;		// Para fazer o gráfico de barras

				break;

			case 1:
			putS("Case1\n\r");
				for(count = 0; count < (100/10); count++){
					delay();

					l = inKey();
					if(l == 'h') mode = 1;
					if(l == 'd') mode = 0;

					if(mode == 1) send2Displays(val);
					else send2Displays(toBcd(val));
				}

				AD1CON1bits.ASAM = 1;
				while(!IFS1bits.AD1IF);
				IFS1bits.AD1IF = 0;
				value = readADC();
				if(mode == 1)
					val = (value*0xFF+511)/ 1023;
				else 
					val = (value*33+511)/ 1023;

				setPWM((value*100+511)/1023);			// PWM on

				LATE &= 0xFFF0;

				break;

			case 2:
			putS("Case2\n\r");
				for(count = 0; count < (100/2); count++){
					delay();

					l = inKey();
					if(l == 'h') mode = 1;
					if(l == 'd') mode = 0;

					if(mode == 1) send2Displays(val);
					else send2Displays(toBcd(val));
				}

				AD1CON1bits.ASAM = 1;
				while(!IFS1bits.AD1IF);
				IFS1bits.AD1IF = 0;

				value = readADC();
				if(mode == 1)
					val = (value*0xFF+511)/ 1023;
				else 
					val = (value*33+511)/ 1023;

				setPWM((value*100+511)/1023);			// PWM on

				LATE &= 0xFFF0;

				if(mode == 1){
					putS("\n\n valor = 0x");
					putc(HexArray[val >> 4]);
					putc(HexArray[val & 0x0F]);
				}
				else {
					putS("\n\n valor = ");
					putc(val/10 + '0');
					putc(val%10 + '0');
				}

				break;

			case 3:
			putS("Case3\n\r");

				setPWM(0);			// PWM off

				LATE &= 0xFFF0;

				
				for(count = 0; count < (100/2); count++){
					TRISDbits.TRISD5 = 1;
					TRISDbits.TRISD6 = 1;
					delay();
				}
				for(count = 0; count < (100/2); count++){
					TRISDbits.TRISD5 = 0;
					TRISDbits.TRISD6 = 0;

					delay();

					l = inKey();
					if(l == 'h') mode = 1;
					if(l == 'd') mode = 0;

					if(mode == 1) send2Displays(val);
					else send2Displays(toBcd(val));
				}

				break;

			default:
				putS("Valor não válido");
				break;

		}
	}

	return 0;
}

void config_IO(void){
	// Configure RB8-RB14 as outputs
	LATB &= 0x80FF;
	TRISB &= 0x80FF;
	// Configure RD5-RD6 as outputs
	LATD &= 0xFF9F;
	TRISD &= 0xFF9F;
	// Configure RE0-RE3 as outputs
	LATE &= 0xFFF0;
	TRISE &= 0xFFF0;
	// Configure RD0 as output
	LATDbits.LATD0 = 0;
	TRISDbits.TRISD0 = 0;
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

void configTimer(void){
	// Configuration of Timer T2
	T2CONbits.TCKPS = 2;
	PR2 = 49999;
	TMR2 = 0;				// Reset timer T2 count register
	T2CONbits.TON = 1;		// Enable timer T2 (Must be the last command of the
							//                  timer configuration sequence!!!)
}

void configUART(void){
	U1MODEbits.BRGH = 0;					// 16x
	U1BRG = (PBCLK+8*baud)/(16*baud)-1;

	U1MODEbits.PDSEL = 0;					// Parity data
	U1MODEbits.STSEL = 0;					// Stop bits -> 1 stop bit
	U1STAbits.UTXEN = 1;					// Ativar transmissão
	U1STAbits.URXEN = 1;					// Ativar receção
	U1MODEbits.ON = 1;
}

void configOC1(void){
	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 0;
	OC1RS = 0;
	OC1CONbits.ON = 1;
}

void setPWM(unsigned char dc){
	if(dc > 100) dc = 100;
	OC1RS = (dc*(PR2+1))/100;
}

void putc(char c){
	while(U1STAbits.UTXBF);
	U1TXREG = c;
}

void putS(char *str){
	while(*str){
		putc(*str);
		str++;
	}
}

char getc(void){
	if(U1STAbits.OERR)
		U1STAbits.OERR == 0;
	while(U1STAbits.URXDA == 0);
	if(U1STAbits.PERR || U1STAbits.FERR){
		char dummy;
		dummy = U1RXREG;
		return 0;
	}
	return U1RXREG;
}

char inKey(void){
	if(U1STAbits.OERR)
		U1STAbits.OERR == 0;
	if(U1STAbits.URXDA){
		if(U1STAbits.PERR || U1STAbits.FERR){
		char dummy;
		dummy = U1RXREG;
		return 0;
		}
		return U1RXREG;
	}
	return 0;	
}

unsigned int readADC(void){
	int *p = (int*)(&ADC1BUF0);
	int soma = 0;
	// Calculate buffer average (8 samples)
	// Calculate voltage amplitude
	int i;
	for(i = 0; i < (AD1CON2bits.SMPI+1); i++){
		soma += p[i*4];
	}
	return soma/(AD1CON2bits.SMPI+1);
}

void send2Displays(unsigned int value){
	static const char display7codes[] = {0x3F, 0x06, 0x5B, 0x4F, 0X66, 0X6D, 0X7D, 0X07,
										 0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71};
	static char flag = 0;
	// Hexadecimal
	unsigned char digit_low = value & 0x0F;
	unsigned char digit_high = value >> 4;

	// Decimal
	//unsigned char digit_low = value%10;
	//unsigned char digit_high = value/10;

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

unsigned int toBcd(unsigned int value){
	return ((value/10)<<4) + (value%10);
}

void delay(void){
	TMR2 = 0;
	while(!IFS0bits.T2IF);
	IFS0bits.T2IF = 0;
}
