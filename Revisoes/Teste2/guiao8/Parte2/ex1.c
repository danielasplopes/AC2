#include <detpic32.h>

void config_UART(void);
void config_interrupts(void);
void putc(char byte2send);

int main(void){
	config_UART();
	config_interrupts();

	while(1);

	return 0;
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

void config_interrupts(void){
	/* UART interrupts */
	IPC6bits.U1IP = 2;					// Priority
	IEC0bits.U1RXIE = 1;				// Receive Interrupt Enable
	IEC0bits.U1TXIE = 0;				// Transmit Interrupt Enable
	IEC0bits.U1EIE = 1;					// Fazer deteção de erros por interrupção

	IFS0bits.U1RXIF = 0;
	IFS0bits.U1TXIF = 0;
	IFS0bits.U1EIF = 0;

	EnableInterrupts();
}

void putc(char byte2send){
	while(U1STAbits.UTXBF);
	U1TXREG = byte2send;
}

void _int_(24) isr_UART1(void){
	putc(U1RXREG);
	IFS0bits.U1RXIF = 0;				// Clear UART1 rx interrupt flag
}
