x = 111
y = 175

calc:
	ldi #y
	sbc #x

	beq equal
	bcs last
	jmp first

equal:
	ldi #0
	jmp out

last:
	ldi #y
	jmp out

first:
	ldi #x
	
out:
	lcd #0x01
	out.d
	hlt