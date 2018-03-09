lcd #0x0c
lcd #0x01

loop:
	ipr
	adc #0
	beq loop

	cmp #0x04
	lcd #0xff
	beq left

right:
	out.c #127
	jmp loop


left:
	out.c #126
	jmp loop
