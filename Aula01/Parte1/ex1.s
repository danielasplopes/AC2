		.equ	PRINT_STR, 8
		.data
msg: 	.asciiz "AC2 - DEPTIC32\n"
		.text
		.globl main

main:	la	$a0, msg
		li 	$v0, PRINT_STR
		syscall							# printStr(msg)
		li	$v0, 0						# return 0;
		jr	$ra
