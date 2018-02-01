;Adding loop, and stop if carry bit is set

x = 7

ldi #x
sta 0x80

start:
	out #0x04
	add 0x80
	bcs stop
	jmp start
stop:
	hlt