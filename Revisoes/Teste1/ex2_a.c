/*
	2.
	a) Um programa que apresente nos display + significativo uma
	forma animada que simule uma cobra (snake), correspondendo
	à sucessiva ativação dos segmentos f, a, b, g, e, d, c, g um 
	de cada vez.
	Cada segmento deve estar ativo durante 0,25 segundos.
*/

#include <detpic32.h>

#define seg_A (0x1)
#define seg_B (0x2)
#define seg_C (0x4)
#define seg_D (0x8)
#define seg_E (0x10)
#define seg_F (0x20)
#define seg_G (0x40)

#define MSD (0x40)
#define LSD (0x20)

void configIO(void);
void delay(int ms);

int main(void){
	configIO();
	const unsigned char snake[] = {seg_F, seg_A, seg_B, seg_G, seg_E, seg_D,
								   seg_C, seg_G};
	unsigned char i = 0;

	LATD |= MSD;		// Ativa o display + significativo

	while(1){
		for(i = 0; i < 8; i++){
			LATB &= 0x80FF;		// limpar segmento
			LATB |= snake[i] << 8;
			delay(250);			// 0,25 segundos
		}
	}
	return 0;
}

void configIO(void){
	// Configure RD6-RD5 as outputs
	LATD &= 0xFF9F;
	TRISD &= 0xFF9F;
	// Configure RB8-RB14 as outputs
	LATB &= 0x80FF;
	TRISB &= 0x80FF;
}

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() < PBCLK/1000);
	}
}

