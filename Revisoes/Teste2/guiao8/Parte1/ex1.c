#include <detpic32.h>

void config_UART(void);

unsigned int baud = 115200;

int main(void){
	config_UART();

	while(1);
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
