# include <detpic32.h>

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

void comDrv_config(unsigned int baud, char parity, int databits, unsigned int stopbits);
void config_interrupts(void);
void comDrv_flushRx(void);
void comDrv_flushTx(void);
void comDrv_putc(char ch);
void comDrv_putS(char *s);

int main(void){
	comDrv_config(115200, 'N', 8, 1);	// default pterm parameters (with TX and RX interrupts disabled)
	comDrv_flushRx();
	comDrv_flushTx();
	config_interrupts();

	while(1){
		comDrv_putS("Teste do bloco de transmissão do device driver!...\n");
	}

	return 0;
}

void comDrv_config(unsigned int baud, char parity, int databits, unsigned int stopbits){
	if(baud < 600 || baud > 115200) baud = 115200;

	U1MODEbits.BRGH = 0;					// fator de divisão do relógio = 16
	U1BRG = (PBCLK+8*baud)/(16*baud)-1;

	if(databits == 9)
		U1MODEbits.PDSEL = 11;
	else{
		if(parity == 'E')
			U1MODEbits.PDSEL = 01;
		else if(parity == 'O')
			U1MODEbits.PDSEL = 10;
		else
			U1MODEbits.PDSEL = 00;
	}
	
	if(stopbits == 2)
		U1MODEbits.STSEL = 1;					
	else U1MODEbits.PDSEL = 0;

	U1STAbits.UTXEN = 1;					// Ativar transmissão
	U1STAbits.URXEN = 1;					// Ativar receção
	U1MODEbits.ON = 1;
}

void config_interrupts(void){
	/*** UART interruptions ***/
	IPC6bits.U1IP = 2;					// Priority bits
	IEC0bits.U1TXIE = 0;				// Transmit interrupt enable
	IEC0bits.U1RXIE = 0;				// Receive interrupt enable
	IEC0bits.U1EIE = 1;					// Fazer deteção de erros por interrupção
	// Interrupt flag bits 
	IFS0bits.U1TXIF = 0;				
	IFS0bits.U1RXIF = 0;
	IFS0bits.U1EIF = 0;

	EnableInterrupts();			// Global Interrupt Enable
}

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
void comDrv_putS(char *s){
	while(*s != '\0'){
		comDrv_putc(*s);
		s++;
	}
}

char comDrv_getC(char *pchar){
	if(rxb.count == 0) return FALSE;				// Test "count" (reception buffer) and return false
	DisableUart1RxInterrupt();						// Begin critical section
	*pchar = rxb.data[rxb.head];					// Copy character pointed by "head" to *pchar
	rxb.count--;									// Decrement "count"
	rxb.head = (rxb.head + 1) $ INDEX_MASK;			// Increment "head"
	EnableUart1RxInterrupt();						// End of critical section
	return TRUE;
}

void _int_(24) isr_uart1(void){
	if(IFS0bits.U1TXIF){							// if U1TXIF is set
		if(txb.count > 0){							// if "count"(transmission buffer, txb) is > 0
			U1TXREG = txb.data[txb.head];			// Copy character poited by "head" to U1TXREG register
			txb.head = (txb.head + 1) & INDEX_MASK;	// Increment "head" variable (mod BUF_SIZE)
			txb.count--;							// Decrement "count" variable
		}
		if(txb.count == 0)							// id "count" variable = 0
			DisableUart1TxInterrupt();
		IFS0bits.U1TXIF = 0;					// Reset UART1 TX interrrupt flag							
	}
}
