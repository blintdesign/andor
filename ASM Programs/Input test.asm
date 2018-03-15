lcd #0x0c
lcd #0x01

loop:
	ipr
	adc #0
	beq loop

	lcd #0xff

	cmp #0x08
	beq left
	cmp #0x01
	beq right
	cmp #0x02
	beq up
	cmp #0x04
	beq down
	cmp #0x03
	beq btn_a
	cmp #0x06
	beq btn_b
	cmp #11
	beq select
	jmp start
right:
	out.c #0x4C	; R
	jmp loop
left:
	out.c #0x52	; L
	jmp loop
up:
	out.c #0x55	; U
	jmp loop
down:
	out.c #0x44	; D
	jmp loop
btn_a:
	out.c #0x41	; A
	jmp loop
btn_b:
	out.c #0x42	; B
	jmp loop
select:
	out.d #1	; 1
	jmp loop
start:
	out.d #2	; 2
	jmp loop