	.equ	SFR_BASE_HI, 0xBF88

	.equ	READ_CORE_TIMER, 11
	.equ	RESET_CORE_TIMER, 12

	.equ	TRISE, 0x6100
	.equ	PORTE, 0x6110
	.equ	LATE, 0x6120

	.equ	inKey, 1

	.data
	.text
	.globl main

main:	lui	$t0, SFR_BASE_HI
		lw	$t1, TRISE($t0)
		andi	$t1, $t1, 0xFFF0
		sw	$t1, TRISE($t0)
		li 	$t2, 0					# time counter

while:	li 	$v0, inKey
		syscall						# Lê valor do teclado

		beq $v0, 0, else			# if (valor != 0)
		li 	$t2, 0					# reset time counter
		bgt	$v0, 'z', endtest1
		blt	$v0, 'a', endtest1

write:	lw	$t1, LATE($t0)
		andi	$t1, $t1, 0xFFF0
		andi	$t3, $v0, 0x000F
		or	$t1, $t1, $t3 		# Acende todos os leds
		sw	$t1, LATE($t0)
		j 	endproc

endtest1:	bgt	$v0, 'Z', endtest
			blt	$v0, 'A', endtest
			j 	write

endtest:	lw	$t1, LATE($t0)
			ori	$t1, $t1, 0x000F
			sw	$t1, LATE($t0)

endproc:	j 	endif

else:	addiu	$t2, $t2, 1			# time counter++

endif:	blt	$t2, 100, endif2
		li 	$t2, 100
		lw	$t1, LATE($t0)
		andi	$t1, $t1, 0xFFF0
		sw	$t1, LATE($t0)

endif2:	li 	$v0, RESET_CORE_TIMER
		syscall

while1: li 	$v0, READ_CORE_TIMER
		syscall
		blt	$v0, 200000, while1
		addi 	$a0, $a0, -1
		j 	while

		jr	$ra
