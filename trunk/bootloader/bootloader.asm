; Copyright (C) 2008 Luca Salmin
; Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
;
; This file is part of ClearOS.
;
; ClearOS is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; ClearOS is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with ClearOS.  If not, see <http://www.gnu.org/licenses/>.

KERNEL_SIZE	equ	0x10
KERNEL_START	equ	0x1000

SECTION .text
  org 0x7C00

init:
	xor ax,ax	;azzero
	mov ds,ax	;setto il descrittore a 0
	mov ss,ax	;preparo lo stack
	mov sp,0x9c00	;preparo lo stack
start:
	mov si, KERNEL_SIZE
	mov bx,ax
	mov ax, KERNEL_START
	mov es, ax
	mov cx, 2
	xor dx,dx
	
READLOOP:
	mov ah,2
	mov al,1
	int 0x13
	jc short ERROR
	mov ax,es	;
	add ax,32	;incrementa di 32 byte es
	mov es,ax	;
	dec si
	jz short READEND
	inc cx
	cmp cl,18
	jbe short READLOOP
	mov cl,1
	inc dh
	cmp dh,2
	jne short READLOOP
	mov dh,0
	inc ch
	jmp short READLOOP

ERROR:
	xor ax,ax
	int 0x16
	int 0x19

READEND:
	mov edx, 0x3f2
	mov al,0x0c
	out dx,al	;spegne il floppy

PMODE:

	mov eax,cr0   ;  metto in eax cr0
	or  al,1      ;  imposto bit per pmode
	mov cr0,eax   ;  entro in modalita protetta
[BITS 32]
	;mov ax,'X'
	;mov [0xb8000],ax
	
	jmp KERNEL_START;salto al kernel
	
; Fine
times 510-($-$$) db 0		; Riempe il resto di 0
db 0x55
db 0xAA			; Aggiunge la firma del bootloader per marcare il device come avviabile
