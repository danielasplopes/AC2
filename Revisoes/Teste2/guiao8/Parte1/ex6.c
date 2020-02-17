#include <detpic32.h>

void config_IO(void);
void config_UART(unsigned int baud, char parity, int databits, unsigned int stopbits);
void putc(char byte2send);
void putS(char *str);

int main(void){
	config_IO();
	config_UART(115200, 'N', 8, 1);

	while(1){
		/*
			Vamos medir o tempo que a UART demora a transmitir strings com diferente dimensão,
			partindo sempre da uma situação de repouso, isto é, em que garantidamente a UART não
			tem nenhuma informação pendente para ser transmitida.
			Para isso vamos usar o bit TRMT do registo UxSTA que, quando a 1, indica que o TX FIFO
			e o transmit shif register estão ambos vazios.
		*/
		while(U1STAbits.TRMT == 0);			// wait until TRMT == 1

		LATBbits.LATB6 = 1;					// Set RB6
		putS("12345\n");
		LATBbits.LATB6 = 0;					// Reset RB6
	}
	return 0;
}

void config_IO(void){
	// Configuration of RB6 as output
	LATB &= 0xFFBF;
	TRISB &= 0xFFBF;
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

