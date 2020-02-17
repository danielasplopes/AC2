	.equ	SFR_BASE_HI, 0xBF88

	.equ	READ_CORE_TIMER, 11
	.equ	RESET_CORE_TIMER, 12

	.equ	TRISE, 0x6100
	.equ	PORTE, 0x6110
	.equ	LATE, 0x6120

	.equ	TRISB, 0x6040
	.equ	PORTB, 0x6050
	.equ	LATB, 0x6060

	.data
	.text
	.globl main

main: 	li $t3, 0	 			# count = 0;
		lui	$t0, SFR_BASE_HI
		# Configuration of RE0-RE3 as outputs
		lw	$t1, TRISE($t0)
		andi	$t1, $t1, 0xFFF0
		sw $t1, TRISE($t0)
		# Configuration of RB0-RB3 as inputs
		lw 	$t1, TRISB($t0)
		ori	$t1, $t1, 0x0008
		sw	$t1, TRISB($t0)

while:	# Lê valores do dip-switch
		lw	$t2, PORTB($t0)
		andi	$t2, $t2, 0x0008

if:		beq	$t2, 0, else			# if(RB3 = 1)
		addi 	$t3, $t3, 1			# 	count++;
		j 	endif

else:	addi	$t3, $t3, -1		# count--;

endif:	# Escreve valores nos leds
		lw	$t4, LATE($t0)
		andi	$t4, $t4, 0xFFF0
		or 	$t4, $t4, $t3
		sw	$t4, LATE($t0)

		li 	$a0, 500				# 500ms -> 2Hz(complete duty-cycle)
		jal delay

		j 	while



#*************** delay() ***************
delay:	beq	$a0, 0, endfor
		li 	$v0, RESET_CORE_TIMER
		syscall

while1: li 	$v0, READ_CORE_TIMER
		syscall
		blt	$v0, 20000, while1
		addi 	$a0, $a0, -1
		j 	delay

endfor: jr	$ra
