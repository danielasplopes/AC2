# Mapa de registos:
#	$t0 : 
	
		.equ	PRINT_STR, 8
		.equ	READ_STR, 9
		.equ	PRINT_INT, 6
		.equ	PRINT_INT10, 7
		.equ	STR_MAX_SIZE, 20
		.data
prompt:	.asciiz	"Introduza 2 strings: "
result:	.asciiz	"Resultados:\n"
str1:	.space	STR_MAX_SIZE+1
str2:	.space	STR_MAX_SIZE+1
str3:	.space	2*STR_MAX_SIZE+1
		.text
		.globl main

main:	subu	$sp, $sp, 4
		sw	$ra, 0($sp)

		la	$a0, prompt
		li 	$v0, PRINT_STR
		syscall							# printStr(prompt);

		li	$v0, READ_STR
		la	$a0, str1
		ĺi	$a1, STR_MAX_SIZE
		syscall							# readStr(str1, STR_MAX_SIZE);

		la	$a0, str2
		li	$a1, STR_MAX_SIZE
		syscall							# readStr(str2, STR_MAX_SIZE);

		la	$a0, result
		li	$v0, PRINT_STR	
		syscall							# printStr(result);

		la	$a0, str1
		jal	strlen						# strlen(str1)
		move	$a0, $v0
		li	$a1, 10
		li	$v0, PRINT_INT
		syscall							# printIt(strlen(str1), 10);
		
		la 	$a0, str2
		jal	strlen
		move 	$a0, $v0
		li	$a1, 10
		li	$v0, PRINT_INT
		syscall							# printInt(strlen(str2), 10);

		la	$a0, str3
		la	$a1, str1
		jal	strcpy						# strcpy(str3, str1);

		la	$a0, str3
		la	$a1, str2
		jal	strcat						# strcat(str3, str2);
		move	$a0, $v0
		li	$v0, PRINT_STR
		syscall							# printInt(strcat(str3, str2));

		la	$a0, str1
		la	$a1, str2
		jal	strcmp						# strcmp(str1, str2);
		move	$a0, $v0
		li	$v0, PRINT_INT10	
		syscall							# print_int10(strcmp(str1, str2));

		li	$v0, 0						# return 0;

		lw	$ra, 0($sp)
		addu	$sp, $sp, 4

		jr	$ra	

################### strlen ######################


################### strcat ######################


################### strcpy ######################


################### strcmp ######################
