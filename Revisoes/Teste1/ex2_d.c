/*
2.
d) Substitua o polling por interrupções.
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
void ADC_config(void);
void configADCInt(void);
void delay(int ms);
unsigned int readADC(void);

unsigned int v_adc = 0;

int main(void){
	configIO();
	ADC_config();
	void configADCInt(void);
	
	const unsigned char snake2[] = {seg_F, seg_A, seg_B, seg_E, seg_D,
								   seg_C, seg_B, seg_A, seg_F, seg_C, seg_D,
								   seg_E};

	const unsigned char snakeDisplay[] = {MSD, MSD, MSD, LSD, LSD, LSD,
										  LSD, LSD, LSD, MSD, MSD, MSD};
	unsigned char i = 0;

	while(1){
		for(i = 0; i < 8; i++){
			AD1CON1bits.ASAM = 1;	// Inicia conversao
			LATD &= 0xFF9F;
			LATD |= snakeDisplay[i];
			LATB &= 0x80FF;		// limpar segmento
			LATB |= snake2[i] << 8;
			delay(250*(v_adc+1));			
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

void ADC_config(void){
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;		// Sample time (always 16!)
	AD1CON2bits.SMPI = 15;		// Number os samples (0 to 15) : 0 means 1 sample
								//								 15 means 16 samples
	AD1CHSbits.CH0SA = 4;		// AN4 = RB4;
	AD1CON1bits.ON = 1;			// ADC on 
}

void configADCInt(void){
	IEC1bits.AD1IE = 1;
	IPC6bits.AD1IP = 1;
	IFS1bits.AD1IF = 0;
	EnableInterrupts();
}

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() < PBCLK/1000);
	}
}

void _int_(27) isr_adc(void){
	v_adc = (readADC() >> 7) & 0x07;
	IFS1bits.AD1IF = 0;
}

unsigned int readADC(void){
	int *p = (int*)(&ADC1BUF0);
	unsigned char i;
	unsigned int soma = 0;

	for(i = 0; i < (AD1CON2bits.SMPI+1); i++){
		soma += p[i*4];
	}

	return (soma/(AD1CON2bits.SMPI+1));
}
