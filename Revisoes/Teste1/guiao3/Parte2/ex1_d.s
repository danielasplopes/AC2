	.equ	SFR_BASE_HI, 0xBF88

	.equ	READ_CORE_TIMER, 11
	.equ	RESET_CORE_TIMER, 12

	.equ	TRISE, 0x6100
	.equ	PORTE, 0x6110
	.equ	LATE, 0x6120

	.data
	.text
	.globl main

main:	li $t3, 0	 			# count = 0;
		lui	$t0, SFR_BASE_HI
		# Configuration of RE0-RE3 as outputs
		lw	$t1, TRISE($t0)
		andi	$t1, $t1, 0xFFF0
		sw $t1, TRISE($t0)

while:	lw	$t2, LATE($t0)
		andi	$t2, $t2, 0xFFF0
		or	$t2, $t2, $t3
		sw	$t2, LATE($t0)

		li 	$a0, 667
		jal	delay

if:		
		


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