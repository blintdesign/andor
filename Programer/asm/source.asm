; Set display

lcd 0x01

ldi #22
sta 0x81

out.d 0x81


hlt
