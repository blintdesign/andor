ldi #1
sta 0x80
ldi #5

lcd #0x01

start:
	lcd #0xff
	out.d
	sbc 0x80
	beq stop
	jmp start
stop:
	hlt
