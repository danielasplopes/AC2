# Mapa de Registos:
#	$t0 : c
#	$t1 : 

		.equ	INKEY, 1
		.equ	PRINT_STR, 8
		.data
msg:	.asciiz "Key pressed\n"
		.text
		.globl main

main:	
while:	li 	$v0, INKEY				
		syscall
		move	$t0, $v0			# c = inkey()

while2:	beq	$t0, 0, while				# while((c = inkey()) == 0){

if:		beq	$t0, '\n', endW			#	if(c == '\n') break;

endW2:	la	$a0, msg
		li	$v0, PRINT_STR
		syscall						#	printStr(msg);
		li	$t0, 0
		j	while2					# }

endW:   li	$v0, 0					# return 0;
		jr	$ra
