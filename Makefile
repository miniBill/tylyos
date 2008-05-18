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

CFLAGS= -march=i386 -ffreestanding -Wall -pedantic -Wextra 
OBJ= kernel/kernel.o drivers/screen/io.o drivers/screen/screen.o memory/memory.o drivers/keyboard/keyboard.o interrupt/interrupt.o bootloader/loader.o memory/gdt.o interrupt/interruptHandler.o interrupt/ldt.o kernel/stdio.o
LDFLAGS= -T linker.ld

all:floppy.img

floppy.img: clearos
	sh make.sh

clearos: $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.asm
	nasm -f elf -o $@ $^

.PHONY: clean
clean:
	rm -f *\~ */*\~ */*.o */*/*\~ */*/*.o log clearos grub.img
