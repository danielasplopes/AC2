#include <detpic32.h>

void configIO(void);
void to7Seg(char c);

int main(void){
	configIO();
	while(1){
		char c = getChar();

		if((c >= 'a' && c <= 'g') || c == '.'){
			to7Seg(c);
		}

		if((c >= 'A' && c <= 'G') || c == '.'){
			to7Seg(c + 32);
		}
	}
	return 0;
}

void configIO(void){
	// Configuration of RB8-RB15 as outputs
	LATB &= 0x00FF;
	TRISB &= 0x00FF;
	// Configuration os RD6-RD5 as outputs
	LATD &= 0xFF9F;
	TRISD &= 0xFF9F;

	LATDbits.LATD5 = 1;
	LATDbits.LATD6 = 0;
}

void to7Seg(char c){
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
