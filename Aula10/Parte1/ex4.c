#include <detpic32.h>

void i2c1_init(unsigned int clock_freq);
void i2c1_start(void);
void i2c1_stop(void);

int main(void){
	
}

void i2c1_init(unsigned int clock_freq){
	I2C1BRG = (PBCLK + clock_freq)/(2 * clock_freq) - 1;	// Config baudrate generator 
	I2C1CONbits.ON = 1;										// Enable I2C1 module
}

void i2c1_start(void){
	while((I2C1CON & 0x1F) != 0);		// Wait until the lower 5 bits of I2C1CON are all 0
	I2C1CONbits.SEN = 1;				// Activate Start event
	while(I2C1CONbits.SEN);				// Wait for completion of the Start event
}

void i2c1_stop(void){
	while((I2C1CON & 0x1F) != 0);		// Wait until the lower 5 bits of I2C1CON are all 0
	I2C1CONbits.PEN = 1;				// Activate Stop event
	while(I2C1CONbits.PEN);				// Wait for completion of the Stop event
}
