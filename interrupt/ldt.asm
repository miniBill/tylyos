global idt_load
extern idt_pointer
idt_load:
    lidt [idt_pointer]
    ret
