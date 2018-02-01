x = 12
y = 14

start:
	ldi #x
	sta 0x09
	ldi #y
	add 0x09
	out #0x04
	nop
	hlt
