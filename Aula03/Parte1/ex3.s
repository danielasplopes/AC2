	.equ 	SFR_BASE_HI, 0xBF88

	.equ	TRISE, 0x6100
	.equ	PORTE, 0x6110
	.equ	LATE, 0x6120

	.equ	TRISB, 0x6040
	.equ	PORTB, 0x6050
	.equ	LATB, 0x6060

	.data
	.text
	.globl main
	
		# Configuração dos portos RE0 a RE3 como saídas
main:	lui	$t0, SFR_BASE_HI
		lw	$t1, TRISE($t0)
		andi	$t1, $t1, 0xFFF0	# RE0 = 0 (0 means OUTPUT)
		sw	$t1, TRISE($t0)
		# Configuração dos portos RB0 a RB3 como entradas
		lw	$t1, TRISB($t0)
		ori	$t1, $t1, 0x000F		# RB0 = 1 (1 means INPUT)
		sw	$t1, TRISB($t0)

while:						# while(1){
		lw	$t2, PORTB($t0)
		andi	$t3, $t2, 0x000F
		xor	$t3, $t3, 0x0009		# inverte os bits de entrada
		lw	$t4, LATE($t0)
		andi $t5, $t4, 0xFFF0
		or 	$t6, $t3, $t5
		sw	$t6, LATE($t0)
		j while
		jr	$ra
		