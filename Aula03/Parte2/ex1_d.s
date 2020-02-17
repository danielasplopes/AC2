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

main:	# Configuração dos portos RE0 a RE3 como saídas
		lui	$t0, SFR_BASE_HI
		lw	$t1, TRISE($t0)
		andi	$t1, $t1, 0xFFF0	# RE0 a RE3 = 0 (0 means OUTPUT)
		sw	$t1, TRISE($t0)
		
		li $t2, 0					# counter = 0;

while:								# while(1){
		# Escrever counter
		lw $t1, LATE($t0)	
		andi	$t1, $t1, 0x000F
		or 	$t1, $t1, $t2
		sw	$t1, LATE($t0)						
		
		andi	$t3, $t2, 0x0008

if:		bne	$t3, 0, else1			#	if(MostSignificantBit == 0){
		sll $t2, $t2, 1				#   sll do contador com 1
		ori	$t2, $t2, 0x0001		#	LeastSignificantBit = 1;
		j 	endif

else1:	sll $t2, $t2, 1				#	sll do contador com 0
		andi	$t2, $t2, 0x000F	#	LeastSignificantBit = 0;
		
endif:	andi	$t2, $t2, 0x000F
		
		li 	$t2, 667
		jal	delay					# delay(375); -> 1.5Hz
		j 	while

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
