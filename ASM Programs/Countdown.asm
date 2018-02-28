ldi #1
sta 0x80
ldi #5

out #0x04

start:
	sub 0x80
	out #0x04
	beq stop
	jmp start
stop:
	hlt
