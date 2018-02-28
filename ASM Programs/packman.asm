jmp start

text:
	#str "GAME OVER"
	len = pc - text
start:
	lcd #0x1
	lcd #0b00001100
	ldi  #0
	out.c #0xfc
map:
	add #1
	out.c #0xa5
	cmp #15
	beq game
	jmp map
game:
	sub #0x01

	lcd #0xce
	out.d
	out.c #0x20

	add #128
	lcd.a
	out.c #0xba
	out.c #0x20
	sub #128
	beq end
	jmp game
end:
	lcd #0x1
	ldi #text
over:
	out.c
	add #0x01
	cmp #len + text
	beq stop
	jmp over
stop:
	hlt
