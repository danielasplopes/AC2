#include <detpic32.h>

void printTo7Seg(char c);

void main(void){
	// The initial value should be set before
	// configuring the port as output
	LATB = LATB & 0x00FF;
	LATD = LATD & 0xFF9F;

	TRISB = TRISB & 0x00FF;			// RB8 a RB15 = 0; (0 means OUTPUT)
	TRISD = TRISD & 0xFF9F;			// RD5 = 1 e RD6 = 0; (0 means OUTPUT)

	// Display low
	LATDbits.LATD5 = 1;				// RD5 = 1;
	LATDbits.LATD6 = 0;				// RD6 = 0;

	// Display  high
/*	LATDbits.LATD5 = 0;				// RD5 = 0;
	LATDbits.LATD6 = 1;				// RD6 = 1;*/

	while(1){
		char c = getChar();

		if((c >= 'a' && c <= 'g') || c == '.') 
			printTo7Seg(c);

		if((c >= 'A' && c <= 'G') || c == '.')
			printTo7Seg(c+32);
	}
}

void printTo7Seg(char c){
	switch(c){
		case 'A': 
		case 'a': 
			LATB = 0x0100;
			break;
		case 'B':
		case 'b':
			LATB = 0x0200;
			break;
		case 'C':
		case 'c':
			LATB = 0x0400;
			break;
		case 'D':
		case 'd':
			LATB = 0x0800;
			break;
		case 'E':
		case 'e':
			LATB = 0x1000;
			break;
		case 'F':
		case 'f':
			LATB = 0x2000;
			break;
		case 'G':
		case 'g':
			LATB = 0x4000;
			break;
		case '.':
			LATB = 0x8000;
			break;
	}
}
