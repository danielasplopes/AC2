	.equ	READ_CORE_TIMER, 11
	.equ	RESET_CORE_TIMER, 12

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

		
main:	li $t2, 0					# count = 0;
		# Configuração dos portos RE0 a RE3 como saídas
		lui	$t0, SFR_BASE_HI
		lw	$t1, TRISE($t0)
		andi	$t1, $t1, 0xFFF0	# RE0 = 0 (0 means OUTPUT)
		sw	$t1, TRISE($t0)
		# Configuração dos portos RB0 a RB3 como entradas
		lw	$t1, TRISB($t0)
		ori	$t1, $t1, 0x000F		# RB0 = 1 (1 means INPUT)
		sw	$t1, TRISB($t0)

while:								# while(1){
		lw	$t3, PORTB($t0)
		andi	$t3, $t3, 0x000F
		addi	$t2, $t2, 1			# 	count++;
		lw	$t4, LATE($t0)
		andi	$t4, $t4, 0xFFF0
		or 	$t4, $t4, $t2
		sw	$t4, LATE($t0)
		li 	$a0, 500
		jal	delay
		j while

#********** delay *************

delay:	beq	$a0, 0, endfor			# while(ms > 0){
		li	$v0, RESET_CORE_TIMER
		syscall						# 	resetCoreTimer();

while1:	li	$v0, READ_CORE_TIMER
		syscall						# 	readCoreTimer();
		blt	$v0, 20000, while1 		#	while(readCoreTimer() < K);
		addi $a0,$a0,-1		
		j	delay 

endfor: jr	$ra
