.extern interrupt_handler

.globl isr_0
.globl isr_1
.globl isr_2
.globl isr_3
.globl isr_4
.globl isr_5
.globl isr_6
.globl isr_7
.globl isr_8
.globl isr_9
.globl isr_10
.globl isr_11
.globl isr_12
.globl isr_13
.globl isr_14
.globl isr_15
.globl isr_16
.globl isr_17
.globl isr_18

.globl isr_32
.globl isr_33
.globl isr_47

isr_0:          # division by zero exception
    cli
    push $0     # Codice di errore fittizio.
    push $0     # Numero di procedura ISR.
    jmp isr_common

isr_1:          # debug exception
    cli
    push $0     # Codice di errore fittizio.
    push $1     # Numero di procedura ISR.
    jmp isr_common

isr_2:          # non maskable interrupt
    cli
    push $0     # Codice di errore fittizio.
    push $2     # Numero di procedura ISR.
    jmp isr_common

isr_3:          # breakpoint
    cli
    push $0     # Codice di errore fittizio.
    push $3     # Numero di procedura ISR.
    jmp isr_common

isr_4:          # into detected overflow
    cli
    push $0     # Codice di errore fittizio.
    push $4     # Numero di procedura ISR.
    jmp isr_common

isr_5:          # out of bounds
    cli
    push $0     # Codice di errore fittizio.
    push $5     # Numero di procedura ISR.
    jmp isr_common

isr_6:          # invalid opcode
    cli
    push $0     # Codice di errore fittizio.
    push $6     # Numero di procedura ISR.
    jmp isr_common

isr_7:          # no coprocessor
    cli
    push $0     # Codice di errore fittizio.
    push $7     # Numero di procedura ISR.
    jmp isr_common

isr_8:          # double fault exception
    cli
    push $0
    push $8     # Numero di procedura ISR.
    jmp isr_common

isr_9:          # coprocessor segment overrun
    cli
    push $0     # Codice di errore fittizio.
    push $9     # Numero di procedura ISR.
    jmp isr_common

isr_10:          # bad TSS
    cli
    push $10     # Numero di procedura ISR.
    jmp isr_common

isr_11:          # segment not present
    cli
    push $11     # Numero di procedura ISR.
    jmp isr_common

isr_12:          # stack fault
    cli
    push $12     # Numero di procedura ISR.
    jmp isr_common

isr_13:          # general protection fault
    cli
    push $13     # Numero di procedura ISR.
    jmp isr_common

isr_14:          # page fault
    cli
    push $14     # Numero di procedura ISR.
    jmp isr_common

isr_15:          # unknown interrupt
    cli
    push $0     # Codice di errore fittizio.
    push $15     # Numero di procedura ISR.
    jmp isr_common

isr_16:          # coprocessor fault
    cli
    push $0     # Codice di errore fittizio.
    push $16     # Numero di procedura ISR.
    jmp isr_common

isr_17:          # alignment check exception
    cli
    push $0     # Codice di errore fittizio.
    push $17     # Numero di procedura ISR.
    jmp isr_common

isr_18:          # machine check exception
    cli
    push $0     # Codice di errore fittizio.
    push $18     # Numero di procedura ISR.
    jmp isr_common


isr_32:         # IRQ 0: timer
    cli
    push $0     # Codice di errore fittizio.
    push $32    # Numero di procedura ISR.
    jmp isr_common

isr_33:         # IRQ 1: tastiera
    cli
    push $0     # Codice di errore fittizio.
    push $33     # Numero di procedura ISR.
    jmp isr_common

isr_47:         # IRQ 15: canale IDE secondario
    cli
    push $0     # Codice di errore fittizio.
    push $47    # Numero di procedura ISR.
    jmp isr_common

isr_common:
    pushl %gs
    pushl %fs
    pushl %es
    pushl %ds
    pushl %edi
    pushl %esi
    pushl %ebp
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    call interrupt_handler
    popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    popl %ebp
    popl %esi
    popl %edi
    popl %ds
    popl %es
    popl %fs
    popl %gs
    add $4, %esp     # Espelle il numero di procedura ISR.
    add $4, %esp     # Espelle il codice di errore (reale o fittizio).
    sti
    iret             # ripristina EIP, CS, EFLAGS, SS
                     # e conclude la procedura.
