# Mapar de Registos
#	$t0 : c

		.equ	GET_CHAR, 2
		.equ	PUT_CHAR, 3
		.data
		.text
		.globl main

main:	
while:	li 	$v0, GET_CHAR
		syscall
		move	$t0, $v0			# c = getChar();

if:		beq	$t0, '\n', endw		# if(c == '\n') break
		move $a0, $t0
		li	$v0, PUT_CHAR
		syscall					# putChar(c);
		j	while

endw:	li	$v0, 1
		jr	$ra
