# Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
#
# This file is part of ClearOS.
#
# ClearOS is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# ClearOS is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with ClearOS.  If not, see <http://www.gnu.org/licenses/>.

CFLAGS= -march=i386 -ffreestanding -Wall -pedantic -Wextra -Werror
OBJ= kernel/kernel.o kernel/stdio.o kernel/screen.o memory/memory.o interrupt/interrupt.o bootloader/loader.o memory/gdt.o interrupt/interruptHandler.o interrupt/ldt.o
LDFLAGS= -T linker.ld

all:floppy.img

floppy.img: clearos
	sh make.sh

kernel/kernel.o: kernel/kernel.h kernel/screen.h kernel/stdio.h memory/memory.h

kernel/stdio.o: kernel/stdio.h

kernel/screen.o: kernel/screen.h

memory/memory.o: memory/memory.h

interrupt/interrupt.o: interrupt/interrupt.h interrupt/interrupt.c
	cc -march=i386 -ffreestanding -Wall -pedantic -Wextra -c -o $@ interrupt/interrupt.c

clearos: $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.asm
	nasm -f elf -o $@ $^

.PHONY: clean
clean:
	rm -f *\~ */*\~ */*.o log clearos grub.img
