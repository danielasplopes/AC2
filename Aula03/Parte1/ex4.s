# Mapa de Registos
#	$t0 : SRF_BASE_HI
#	$t1 : v

	.equ 	SFR_BASE_HI, 0xBF88

	.equ	READ_CORE_TIMER, 11
	.equ	RESET_CORE_TIMER, 12

	.equ	TRISE, 0x6100
	.equ	PORTE, 0x6110
	.equ	LATE, 0x6120

	.data
	.text
	.globl main

main:	li $t1, 0				# v = 0;

		# Configuração do porto RE0 como saída
		lui	$t0, SFR_BASE_HI
		lw	$t3, TRISE($t0)
		andi	$t3, $t3, 0xFFFE	# RE0 = 0 (o means OUTPUT)
		sw	$t3, TRISE($t0)

while:								# while(1){
		lw	$t2, LATE($t0)
		andi	$t2, $t2, 0xFFFE
		or	$t2, $t2, $t1
		sw	$t2, LATE($t0)			# 	LATE0 = v;

		li $a0, 500
		jal	delay					# delay(500);

		xori $t1, $t1, 0x0001		# v ^= 1;
		j while

# ************ delay *************

delay:	beq	$a0, 0, endfor			# while(ms > 0){
		li	$v0, RESET_CORE_TIMER
		syscall						# 	resetCoreTimer();

while1:	li	$v0, READ_CORE_TIMER
		syscall						# 	readCoreTimer();
		blt	$v0, 20000, while1 		#	while(readCoreTimer() < K);
		addi $a0,$a0,-1		
		j	delay 

endfor: jr	$ra
