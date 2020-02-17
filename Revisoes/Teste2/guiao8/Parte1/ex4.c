#include <detpic32.h>

void config_UART(unsigned int baud, char parity, int databits, unsigned int stopbits);
void putc(char byte2send);
void putS(char *str);
void delay(int ms);

int main(void){
	config_UART(1200, 'O', 8, 2);

	while(1){
		putS("String de teste\n");
		delay(1000);		// wait 1 s
	}
	return 0;
}

void config_UART(unsigned int baud, char parity, int databits, unsigned int stopbits){
	// Configurations of baudarate generator
	if(baud < 600 || baud > 115200) baud = 115200;
	U1MODEbits.BRGH = 0;
	U1BRG = ((PBCLK + 8 * baud) / (16 * baud)) - 1;

	// Configuration of the number of data bits, parity and number of stop bits
	if(databits == 9){
		U1MODEbits.PDSEL = 11;
	}
	else{
		if(parity == 'E')
			U1MODEbits.PDSEL = 01;	
		if(parity == 'O')
			U1MODEbits.PDSEL = 10;
		if(parity == 'N')
			U1MODEbits.PDSEL = 00;			
	}
	
	if(stopbits == 1)	
		U1MODEbits.STSEL = 0;
	if(stopbits == 2)
		U1MODEbits.STSEL = 1;

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
