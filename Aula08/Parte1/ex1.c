#include <detpic32.h>

void configUART(unsigned int baud);

int main(void){
	configUART(115200);
}

void configUART(unsigned int baud){
	U1MODEbits.BRGH = 0;					// fator de divisão do relógio = 16
	U1BRG = (PBCLK+8*baud)/(16*baud)-1;

	U1MODEbits.PDSEL = 00;					// Number of bits-Parity data
	U1MODEbits.STSEL = 0;					// Stop bits -> 1 stop bit
	U1STAbits.UTXEN = 1;					// Ativar transmissão
	U1STAbits.URXEN = 1;					// Ativar receção
	U1MODEbits.ON = 1;
}