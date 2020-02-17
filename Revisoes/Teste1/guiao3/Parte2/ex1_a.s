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

main:	li 	$t4, 0				# count = 0;
		lui	$t0, SFR_BASE_HI
		# Configuration of RE0-RE3 as outputs
		lw	$t1, TRISE($t0)
		andi	$t1, $t1, 0xFFF0
		sw	$t1, TRISE($t0)
		# Configuration of RB0-RB3 as inputs
		lw	$t2, TRISB($t0)
		ori	$t2, $t2, 0x000F
		sw	$t2, TRISB($t0)

while:							# while(1){
		lw	$t3, PORTE($t0)
		andi	$t3, $t3, 0xFFF0
		addi	$t4, $t4, 1		# 	count++;
		sw	$t5, LATE($t0)
		andi	$t5, $t5, 0xFFF0
		or 	$t5, $t5, $t4
		sw	$t5, LATE($t0)
		li 	$a0, 500
		jal delay				# 	delay(500); -> 1 Hz
		j 	while


#*************** delay() *************
delay:	beq	$a0, 0, endfor			# while(ms > 0){
		li  $v0, RESET_CORE_TIMER
		syscall						#	resetCoreTimer();

while1:	li 	$v0, READ_CORE_TIMER
		syscall
		blt	$v0, 20000, while1		# 	while(readCoreTimer() < 20000);
		addi	$a0, $a0, -1		#	ms--;
		j	delay					

endfor:	jr	$ra 					# }
