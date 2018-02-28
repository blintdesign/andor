x = 7

start:
	ldi #0

loop:
	out #0x04
	add #x
	bcs stop
	jmp loop
	
stop:
	hlt
	