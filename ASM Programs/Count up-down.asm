b = 0
x = 1
o = 5

ldi #x
sta 0x81

ldi #o
sta 0x82

ldi #b
sta 0x80

up:
	add 0x81
	out #0x04
	cmp 0x82
	beq down
	jmp up
down:
	sub 0x81
	out #0x04
	beq up
	jmp down