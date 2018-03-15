lcd #0x0c
lcd #0x01
ldi #8
sta 0x60

loop:
	ipr
	adc #0
	beq loop
	cmp #8
	beq left
	cmp #1
	beq right
	jmp loop
right:
	lda 0x60
	sbc #1
	jmp move
left:
	lda 0x60
	adc #1		
	jmp move
move:
	sta 0x60
	lcd #0x01
	sbc #0x80
	lcd.a
	out.c #0b11111100
	jmp loop