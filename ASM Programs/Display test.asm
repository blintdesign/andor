lcd #0x38
lcd #0xF
lcd #0x6
lcd #0x1

ldi #213
out.d

out.c #0x20

ldi #5
out.d

out.c #0x20
out.d #0x3

ldi #0x65
sta 0x66
out.c 0x66

ldi #0

hlt
