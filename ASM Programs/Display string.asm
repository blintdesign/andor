jmp start

text:
	#str "Andor I by Blint"
	len = pc - text

start:
	lcd #0x01
	ldi #text

loop:
	out.c
	adc #0x01
	cmp #len + text
	beq stop
	jmp loop

stop:
	hlt
	