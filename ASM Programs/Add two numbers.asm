x = 7
y = 5

start:
	ldi #x
	sta 0x80
	ldi #y
	add 0x80
	out #0x04
	nop
	hlt
