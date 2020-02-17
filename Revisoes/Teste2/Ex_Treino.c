/* 
   Escreva um programa que comece por congifurar a UART1 com 19200bps, 8 data, Even parity, 2 stopbits
   Configure o modelo OC1, com um sinal PWM, 50% duty-cycle e 100Hz e observe o sinal no osciloscopio
   Quando receber (por interrupção da UART) os caracteres '+' e '-' aumente e diminua o valor do duty-cycle
   em 5%
   Mantenha o valor do duty-cycle entre 5 e 95% e envie mensagens adequadas ao utilizador informando de 
   tentativas fora da gama.
   Mostre a percentagem do duty-cycle nos displays de 7 segmentos, com uma frequencia de 100Hz 
   Sem usar stsyem call, a cada segundo envie para o ecra uma string com a mensagem "O valor atual do 
   duty-cycle é xx%
*/

#include <detpic32.h>

void configIO(void);
void config_UART(void);
void config_Timer(void);
void config_PWM(void);
void config_interrupts(void);
void setPWM(unsigned char dc);
void putc(char byte2send);
void putS(char *str);
char inKey(void);
void send2Displays(unsigned int value);

volatile int dutycycle = 5;

int main(void)
{
	configIO();
	config_UART();
	config_Timer();
	config_PWM();
	config_interrupts();

	while(1){
	}

	return 0;
}

void config_UART(void){
	int baud = 19200;
	U1MODEbits.BRGH = 0;
	U1BRG = ((PBCLK + 8 * baud)/(16 * baud)) - 1;
	U1MODEbits.PDSEL = 01;				// 8 databits, even parity
	U1MODEbits.STSEL = 1;				// 2 stopbits
	U1STAbits.UTXEN = 1;				// transmit module enable
	U1STAbits.URXEN = 1;				// receive module enable
	U1MODEbits.ON = 1;					// Enable UART
}

void configIO(void){
	TRISB &= 0x80FF;
	LATB &= 0x80FF;
	TRISD &= 0xFF9F;
	LATD &= 0xFF9F;
}

void config_Timer(void){
	// Config timer T2 - 100Hz
	T2CONbits.TCKPS = 2;
	PR2 = 49999;
	TMR2 = 0;							// reset timer T2 count register
	T2CONbits.TON = 1;					// Enable timer T2
}

void config_PWM(void){
	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 0;				// Use timer T2
	OC1RS = 25000;						// Ton constant for 50% duty-cycle
	OC1CONbits.ON = 1;					// Enable OC1 module
}

void config_interrupts(void){
	// Timer T2 interrupts
	IPC2bits.T2IP = 2;
	IEC0bits.T2IE = 1;
	IFS0bits.T2IF = 0;
	// UART interrupts
	IPC6bits.U1IP = 2;
	IEC0bits.U1RXIE = 1;
	IEC0bits.U1TXIE = 0;
	IEC0bits.U1EIE = 1;

	IFS0bits.U1RXIF = 0;
	IFS0bits.U1TXIF = 0;
	IFS0bits.U1EIF = 0;

	EnableInterrupts();
}

void setPWM(unsigned char dc){
	if(dc < 5) dc = 5;
	if(dc > 95)	dc = 95;
	OC1RS = ((PR2 + 1) * dc)/100;
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

char inKey(void){
	if(U1STAbits.OERR)
		U1STAbits.OERR = 0;
	return U1RXREG;
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

void _int_(24) isr_UART1(void){
	char l;
	l = inkey();
	if(l == '+'){
		dutycycle += 5;
		if(dutycycle > 95){dutycycle = 95;} 
		setPWM(dutycycle);
	}
	else if(l == '-'){
		dutycycle -= 5;
		if(dutycycle < 5){dutycycle = 5;}
		setPWM(dutycycle);
	}
	else {
		putS("Insira '+' ou '-'");
	}

	IFS0bits.U1RXIF = 0;
}

void _int_(8) isr_T2(void){
	static int counter = 0;
	send2Displays(dutycycle);
	if(++counter == 100){
		counter = 0;
		putS("O valor atual do duty-cycle e ");
		putc(0x30 + dutycycle/10);
		putc(0x30 + dutycycle%10);
		putc('%');
		putc('\n');

	}
	IFS0bits.T2IF = 0;
}
