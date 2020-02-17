#include <detpic32.h>

void configUART(void);
void putc(char byte2send);
void puts(unsigned char *);
void delay(int ms);

unsigned int baud = 115200;

int main(void){
	unsigned char s[] = "String de teste\n";
	configUART();

	while(1){
		puts(s);
		delay(1000);
	}

	return 0;
}

void configUART(void){
	U1MODEbits.BRGH = 0;					// fator de divisão do relógio = 16
	U1BRG = (PBCLK+8*baud)/(16*baud)-1;

	U1MODEbits.PDSEL = 00;					// Number of bits-Parity data
	U1MODEbits.STSEL = 0;					// Stop bits -> 1 stop bit
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
