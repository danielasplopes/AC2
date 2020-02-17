	.equ	SFR_BASE_HI, 0xBF88

	.equ	READ_CORE_TIMER, 11
	.equ	RESET_CORE_TIMER, 12

	.equ	TRISE, 0x6100
	.equ	PORTE, 0x6110
	.equ	LATE, 0x6120

	.data
	.text
	.globl main

main:	li $t2, 0			# v = 0;
		lui	$t0, SFR_BASE_HI
		# Configuration of RE0 as output
		lw	$t1, TRISE($t0)
		andi	$t1, $t1, 0xFFFE
		sw	$t1, TRISE($t0)

while:						# while(1){
		# LATE0 = v;
		lw	$t3, LATE($t0)
		andi	$t3, $t3, 0xFFFE
		or	$t3, $t3, $t2
		sw	$t3, LATE($t0)
		# Atraso de 500 ms
		li 	$a0, 500
		jal delay
		# v ^= 1;
		xori	$t2, $t2, 0x0001

		j 	while 			# }


#****************** delay() *********************
# void delay(int ms){
#	for(; ms > 0; ms--){
#		resetCoreTimer();
#		while(readCoreTimer() < 20000);
#	}
#}

delay:	beq	$a0, 0, endfor		# while(ms > 0){
		li 	$v0, RESET_CORE_TIMER
		syscall					# 	resetCoreTimer();

while1:	li 	$v0, READ_CORE_TIMER
		syscall
		blt	$v0, 20000, while1	# 	while(readCoreTimer() < 20000);
		addi	$a0, $a0, -1	# 	ms--;
		j 	delay

endfor:	jr	$ra
