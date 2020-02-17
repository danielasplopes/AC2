#include <detpic32.h>

void i2c1_init(unsigned int clock_freq);
void i2c1_start(void);
void i2c1_stop(void);
int i2c1_send(unsigned char value);
char i2c1_receive(char ack_bit);

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

int i2c1_send(unsigned char value){
	I2C1TRN = value;					// Copy value to I2C1TRN register
	while(I2C1STATbits.TRSTAT);			// Wait while master transmission is in progress
	return I2C1STATbits.ACKSTAT;		// Return acknowledge status bit
}

char i2c1_receive(char ack_bit){
	while((I2C1CON & 0x1F) != 0);		// Wait until the lower 5 bits of I2C1CON are all 0
	I2C1CONbits.RCEN = 1;				// Activate RCEN bit(receive enable bit)
	while(I2C1STATbits.RBF == 0);		// Wait while byte not received 

	// Copy "ack_bit" to I2C1CON, bit ACKDT
	if(ack_bit)
		I2C1CONbits.ACKDT = 1;
	else I2C1CONbits.ACKDT = 0;
	while((I2C1CON & 0x1F) != 0);		// Wait until the lower 5 bits of I2C1CON are all 0
	I2C1CONbits.ACKEN = 1;				// Start Acknowledge sequence

	while(I2C1CONbits.ACKEN);			// Wait for completion of Acknowledge sequence

	return I2C1RCV;						// Return received value	
}
