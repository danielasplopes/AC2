# Mapar de Registos
#	$t0 : value

		.equ	PRINT_STR, 8
		.equ	PRINT_INT, 6
		.equ	PRINT_INT10, 7
		.equ	READ_INT10, 5 
		.data
str1:	.asciiz	"\nIntroduza um numero (sinal e modulo): "
str2:	.asciiz	"\nValor lido em base 2: "
str3:	.asciiz	"\nValor lido em base 16: "
str4:	.asciiz	"\nValor lido em base 10 (unsigned): "
str5:	.asciiz	"\nValor lido em base 10 (signed): "
		.text
		.globl main

main:	
while:	la	$a0, str1
		li 	$v0, PRINT_STR	
		syscall						# printStr("\nIntroduza um numero (sinal e modulo): ")

		li 	$v0, READ_INT10
		syscall
		move	$t0, $v0			# value = read_int10();

		la	$a0, str2
		li 	$v0, PRINT_STR	
		syscall						# printStr("\nValor lido em base 2: ")

		move	$a0, $t0
		li		$a1, 2
		li 	$v0, 	PRINT_INT
		syscall						# printInt(value, 2);


		la	$a0, str3
		li 	$v0, PRINT_STR	
		syscall						# printStr("\nValor lido em base 16: ")

		move	$a0, $t0
		li		$a1, 16
		li 	$v0, 	PRINT_INT
		syscall						# printInt(value, 16);

		la	$a0, str4
		li 	$v0, PRINT_STR	
		syscall						# printStr("\nValor lido em base 10 (unsigned): ")

		move	$a0, $t0
		li		$a1, 10
		li 	$v0, 	PRINT_INT
		syscall						# printInt(value, 10);

		la	$a0, str5
		li 	$v0, PRINT_STR	
		syscall						# printStr("\nValor lido em base 10 (signed): ")

		move	$a0, $t0
		li 	$v0, 	PRINT_INT10
		syscall						# printInt(value);

		jr	$ra
