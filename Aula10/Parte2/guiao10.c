#include <detpic32.h>
#include "i2c.h"

int getTemperature(int *temperature);
void delay(int ms);

int main(void){
	int temperature;
	i2c1_init(TC74_CLK_FREQ);

	while(1){
		if(getTemperature(&temperature)) return 1;

		printInt10(temperature);	// Print "temperature"
		printf("\n");
		delay(250);
	}

	return 0;
}

int getTemperature(int *temperature){
	int ack;
	i2c1_start();				// Send start event
	ack = i2c1_send(ADDR_WR);	// Send address + WR; copy return value to "ack"
	ack += i2c1_send(RTR);		// Send command; add return value to "ack"
	i2c1_start();				// Send start event again
	ack += i2c1_send(ADDR_RD);	// Send address + RD; add return value to "ack"
	// Test "ack" variable
	if(ack != 0){
		i2c1_stop();
		printf("An error as occured!\n");
	}
	*temperature = i2c1_receive(I2C_NACK);	// Receive a value from slave; copy received
											// value to "temperature"
	i2c1_stop();				// Send stop event
	return ack;
}

void delay(int ms){
	for(; ms > 0; ms--){
		resetCoreTimer();
		while(readCoreTimer() > 20000);
	}
}

