[global read_PIT_count]

read_PIT_count:
	pushfd
	cli
	mov al, 00000000b    ; al = channel in bits 6 and 7, remaining bits clear
	out 0x43, al         ; Send the latch command
 
	in al, 0x40          ; al = low byte of count
	mov ah, al           ; ah = low byte of count
	in al, 0x40          ; al = high byte of count
	rol ax, 8            ; al = low byte, ah = high byte (ax = current count)
	popfd
	ret
