x = 111
y = 75

ldi #x
sta 0x80

ldi #y
sub 0x80

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
	jmp out

out:
	out #0x04
	nop
	hlt
