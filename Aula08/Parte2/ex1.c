#include <detpic32.h>

void configUART(unsigned int baud, char parity, int databits, unsigned int stopbits);
void config_interrupts(void);
void putc(char byte2send);
void putS(unsigned char *str);
char getc(void);

int main(void){
	
	configUART(115200, 'N', 8, 1);
	config_interrupts();

	while(1);

	return 0;
}

void configUART(unsigned int baud, char parity, int databits, unsigned int stopbits){
	if(baud < 600 || baud > 115200) baud = 115200;

	U1MODEbits.BRGH = 0;					// fator de divisão do relógio = 16
	U1BRG = (PBCLK+8*baud)/(16*baud)-1;

	if(databits == 9)
		U1MODEbits.PDSEL = 11;
	else{
		if(parity == 'E')
			U1MODEbits.PDSEL = 01;
		else if(parity == 'O')
			U1MODEbits.PDSEL = 10;
		else
			U1MODEbits.PDSEL = 00;
	}
	
	if(stopbits == 2)
		U1MODEbits.STSEL = 1;					
	else U1MODEbits.PDSEL = 0;

	U1STAbits.UTXEN = 1;					// Ativar transmissão
	U1STAbits.URXEN = 1;					// Ativar receção
	U1MODEbits.ON = 1;
}

void config_interrupts(void){
	/*** UART interruptions ***/
	IPC6bits.U1IP = 2;					// Priority bits
	IEC0bits.U1TXIE = 0;				// Transmit interrupt enable
	IEC0bits.U1RXIE = 1;				// Receive interrupt enable
	IEC0bits.U1EIE = 1;					// Fazer deteção de erros por interrupção
	// Interrupt flag bits 
	IFS0bits.U1TXIF = 0;				
	IFS0bits.U1RXIF = 0;
	IFS0bits.U1EIF = 0;

	EnableInterrupts();			// Global Interrupt Enable
}

void putc(char byte2send){
	while(U1STAbits.UTXBF);
	U1TXREG = byte2send;
}

void putS(unsigned char *str){
	while(*str != '\0'){
		putc(*str);
		str++;
	}
}

char getc(void){
	if(U1STAbits.OERR)
		U1STAbits.OERR = 0;
	while(U1STAbits.URXDA == 0);
	if(U1STAbits.FERR || U1STAbits.PERR){
		char c;
		c = U1RXREG;
		return 0;
	}  
	return U1RXREG;
}

void _int_(24) isr_uart1(void){
	putc(U1RXREG);
	IFS0bits.U1RXIF = 0;
}
