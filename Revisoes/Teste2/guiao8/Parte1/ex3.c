#include <detpic32.h>

void config_UART(void);
void putc(char byte2send);
void putS(char *str);
void delay(int ms);

unsigned int baud = 115200;

int main(void){
	config_UART();

	while(1){
		putS("String de teste\n");
		delay(1000);		// wait 1 s
	}
	return 0;
}

void config_UART(void){
	// Configurations of baudarate generator
	U1MODEbits.BRGH = 0;
	U1BRG = ((PBCLK + 8 * baud) / (16 * baud)) - 1;
	// Configuration of the number of data bits, parity ans number of stop bits
	U1MODEbits.PDSEL = 00;		
	U1MODEbits.STSEL = 0;
	// Enable the transmitter and receiver modules
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	// Enable UART
	U1MODEbits.ON = 1;
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

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() < 20000);
	}
}
