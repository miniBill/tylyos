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

CFLAGS:= -march=i386 -ffreestanding -Wall -pedantic -Wextra -I. -Werror -fno-stack-protector
OBJ= bootloader/loader.o \
     kernel/stdio.o kernel/kernel.o \
     lib/string.o \
     memory/memory.o memory/gdt.o memory/paging.o \
       drivers/screen/screen.o \
       drivers/keyboard/keyboard.o \
       drivers/timer/timer.o \
     interrupt/interrupt.o interrupt/interruptHandler.o interrupt/ldt.o \
     gui/gui.o
LDFLAGS= -T linker.ld

all:iso.img

iso.img: clearos
	mkdir iso && \
mkdir -p iso/boot/grub && \
cp stage2_eltorito iso/boot/grub && \
echo "timeout 1">>iso/boot/grub/menu.lst && \
echo "title   ClearOS">>iso/boot/grub/menu.lst && \
echo "kernel  /boot/clearos">>iso/boot/grub/menu.lst && \
cp clearos iso/boot/clearos && \
mkisofs -quiet -R -b boot/grub/stage2_eltorito -no-emul-boot \
-boot-load-size 4 -boot-info-table -o iso.img iso && \
echo "Iso created" && \
rm -R iso

clearos: $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.asm
	nasm -f elf -o $@ $^

.PHONY: clean
clean:
	rm -f *\~ */*\~ */*.o */*/*\~ */*/*.o log clearos iso.img clearos.iso clearos.iso.sig

.PHONY: smallclean
smallclean:
	rm -f *\~ */*\~ */*.o */*/*\~ */*/*.o log clearos clearos.iso clearos.iso.sig

