/*
	Escreva um programa que leia num ciclo infinito, os
	4 dip_switch. Considere que o valor lido é um número
	em complemento para 1 (na gama -7 a 7). Apresente esse valor
	nos leds, em sinal e modulo, sob forma de um grafico de 
	barras. Os leds LED6..0 devem indicar o modulo (LED6 é o + 
	significativo) e o led 7 deve indicar o sinal (LED7 aceso 
	quando o numero lido for negativo)
*/

void configIO(void);

#include <detpic32.h>

int main(void){
	int dip_switch, modulo, sinal;
	configIO();
	while(1){
		dip_switch = PORTB & 0x000F;
		if(dip_switch & 0x8){	// if MSB = 1
			modulo = ((dip_switch & 0x7)^0x7) & 0x07;	// Inverte os bits (comp 1)
		} else{
			modulo = dip_switch & 0x07;	// Apenas conta os 3 LSB
		}
		sinal = (dip_switch >> 3) & 0x1;
		// Escrever nos leds
		LATE &= 0xFFF0;
		LATEbits.LATE3 = sinal;
		LATE |= ((1 << modulo) -1) & 0x07;
	}
}

void configIO(void){
	// Configure RE0 - RE3 as outputs
	LATE &= 0xFFF0;
	TRISE &= 0xFFF0;
	// Configure RB0-RB3 as inputs
	TRISB |= 0x000F;
}

