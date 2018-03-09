x = 7

start:
	lcd #0x01
	ldi #0

loop:
	lcd #0xff
	out.d
	adc #x
	bcs stop
	jmp loop
	
stop:
	hlt
