global gdtFlush
extern gdtPointer



gdtFlush:
    lgdt [gdtPointer]
    push ebp
    mov ebp,esp

    mov ax,[ebp+12] ;selettore segmento dati (secondo parametro)
    mov bx,[ebp+8] ;selettore segmento codice (primo parametro)

    mov esp,ebp
    pop ebp
    ;mov ax, 0x10 ;segmento 2    TI 0=gdt    RPL=0 backup sempre meglio tenere 
    ;setta tutti i registri di segmento
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ;per settare CS è neccessario utilizzare jmp
    ;jmp 0x08:flush2 ;backup sempre meglio tenere ;P
    push ebx ;retf legge dallo stack i valori
    push flush2
    retf
flush2:
    ret


 jmp 0x30:0x0
