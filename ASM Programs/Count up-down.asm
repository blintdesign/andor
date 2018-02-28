from = 1
to   = 20

start:
	ldi #from

up:
	add #1
	lcd #0x1
	out.d
	cmp #to
	beq down
	jmp up
	
down:
	sub #1
	lcd #0x1
	out.d
	cmp #from
	beq up
	jmp down
