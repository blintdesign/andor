from = 1
to   = 20

start:
	ldi #from

up:
	adc #1
	lcd #0x1
	out.d
	cmp #to
	beq down
	jmp up
	
down:
	sbc #1
	lcd #0x1
	out.d
	cmp #from
	beq up
	jmp down
