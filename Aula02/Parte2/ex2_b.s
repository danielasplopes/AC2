	.equ	READ_CORE_TIMER, 11
	.equ	RESET_CORE_TIMER, 12	
	.data
	.text
	.globl main

main:	move $a0, $t0

for:	ble	$t0, 0, endFor			# while(ms > 0){
		li	$v0, RESET_CORE_TIMER
		syscall						# 	resetCoreTimer();

while:	li	$v0, READ_CORE_TIMER
		syscall						# 	readCoreTimer();
		bge	$v0, 20000, while 		#	while(readCoreTimer() < K);
		j	for 

endfor: jr	$ra