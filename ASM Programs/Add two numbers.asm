x = 123
y = 106

math:
	ldi #x
	add #y

out:
	lcd #0x1
	out.d #x
	out.c 0x2b
	out.d #y
	out.c 0x3d
	out.d

end:
	hlt