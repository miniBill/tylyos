ENTRY (_loader)

SECTIONS{
    . = 0x00100000;

    .text :{
        *(.text)
    }

    .rodata ALIGN (0x1000) : {
        *(.rodata)
 
    }


    .data ALIGN (0x1000) : {
        *(.data)
       
    }
       
    . = ALIGN (0x1000);
    l_pageDir = . ;
    . += 0x1000;/*pageDir*/

    . += 0x400000;/*page tables*/

    l_tempPage = . ;
    . += 0x1000; 

    .bss : {
        _sbss = .;
        *(COMMON)
        *(.bss)
        _ebss = .;
    }

    l_end = .;
}
