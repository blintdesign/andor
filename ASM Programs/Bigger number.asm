x = 111
y = 75

calc:
	ldi #y
	sub #x

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
	out #0x04
	hlt