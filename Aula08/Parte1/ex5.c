#include <detpic32.h>

void configUART(unsigned int baud, char parity, int databits, unsigned int stopbits);
void putc(char byte2send);
void puts(unsigned char *);
void delay(int ms);

unsigned int baud = 115200;

int main(void){
	unsigned char s[] = "String de teste\n";
	configUART(600, 'N', 8, 1);

	while(1){
		puts(s);
		delay(1000);
	}

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

void putc(char byte2send){
	while(U1STAbits.UTXBF);
	U1TXREG = byte2send;
}

void puts(unsigned char *str){
	while(*str != '\0'){
		putc(*str);
		str++;
	}
}

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() < PBCLK/1000);
	}
	
}
