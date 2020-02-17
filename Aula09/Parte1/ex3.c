# include <detpic32.h>

void comDrv_flushRx(void);
void comDrv_flushTx(void);

#define DisableUart1RxInterrupt()	IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt()	IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt()	IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt()	IEC0bits.U1TXIE = 1

#define BUF_SIZE	8

typedef struct {
	unsigned char data[BUF_SIZE];
	unsigned int head;
	unsigned int tail;
	unsigned int count;
} circularBuffer;

volatile circularBuffer txb;			// Transmission buffer
volatile circularBuffer rxb;			// Reception buffer

void comDrv_flushRx(void){
	// Initialize variables of the reception buffer
	rxb.head = 0;
	rxb.tail = 0;
	rxb.count = 0;
}

void comDrv_flushTx(void){
	// Initialize variables of the transmission buffer
	txb.head = 0;
	txb.tail = 0;
	txb.count = 0;
}
