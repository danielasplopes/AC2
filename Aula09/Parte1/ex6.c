# include <detpic32.h>

void comDrv_flushRx(void);
void comDrv_flushTx(void);
void comDrv_putc(char ch);
void comDvr_putS(char *s);

#define DisableUart1RxInterrupt()	IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt()	IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt()	IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt()	IEC0bits.U1TXIE = 1

#define BUF_SIZE	8
#define INDEX_MASK	(BUF_SIZE - 1)

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

// Escreve um caracter no buffer de transmissão e atualiza as variáveis tail e count
void comDrv_putc(char ch){
	while(txb.count == BUF_SIZE);			// wait while buffer is full
	txb.data[txb.tail] = ch;				// Copy character to the transmission buffer at position tail
	txb.tail = (txb.tail + 1) & INDEX_MASK;	// Increment "tail" index (mod. BUF_SIZE)
	DisableUart1TxInterrupt();				// Begin critical section
	txb.count += 1;							// Increment "count" variable
	EnableUart1TxInterrupt();				// End critical section
}

// Evoca a função comDvr_putc() para enviar para a linha série uma string
// (terminada com o caracter '\0')
void comDvr_putS(char *s){
	while(*s != '\0'){
		comDvr_putc(*s);
		s++;
	}
}

void _int_(24) isr_uart1(void){
	if(IFS0bits.U1TXIF){							// if U1TXIF is set
		if(txb.count > 0){							// if "count" variable (transmission buffer, txb) is > 0
			U1TXREG = txb.data[txb.head];			// Copy character poited by "head" to U1TXREG register
			txb.head = (txb.head + 1) & INDEX_MASK;	// Increment "head" variable (mod BUF_SIZE)
			txb.count--;							// Decrement "count" variable
		}
		if(txb.count = 0)							// id "count" variable = 0
			DisableUart1TxInterrupt();
		IFS0bits.U1TXIF = 0;					// Reset UART1 TX interrrupt flag							
	}
}
