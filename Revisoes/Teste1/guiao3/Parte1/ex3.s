	.equ	SFR_BASE_HI, 0xBF88

	.equ	TRISE, 0x6100
	.equ	PORTE, 0x6110
	.equ	LATE, 0x6120

	.equ	TRISB, 0x6040
	.equ	PORTB, 0x6050
	.equ	LATB, 0x6060

	.data
	.text
	.globl main

main:	lui	$t0, SFR_BASE_HI
		# Configuration of RE0-RE3 as outputs
		lw	$t1, TRISE($t0)
		andi	$t1, $t1, 0xFFF0
		sw	$t1, TRISE($t0)
		# Configuration of RB0-RB3 as inputs
		lw	$t1, TRISB($t0)
		ori	$t1, $t1, 0x000F
		sw	$t1, TRISB($t0)

while:	# Lê o valor do porto RB0
		lw	$t2, PORTB($t0)			
		andi	$t3, $t2, 0x000F
		# Escreve o valor no porto RE0
		lw	$t4, LATE($t0)
		andi	$t5, $t4, 0xFFF0
		or 	$t6, $t3, $t5
		xor	$t6, $t6, 0x0009		# RE0 = \RB0; RE1 = RB1; RE2 = RB2; RE3 = \RB3
		sw	$t6, LATE($t0)
		j while
		jr	$ra
