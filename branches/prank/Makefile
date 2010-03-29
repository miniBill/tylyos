# Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
#
# This file is part of TylyOS.
#
# TylyOS is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# TylyOS is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with TylyOS.  If not, see <http://www.gnu.org/licenses/>.

CFLAGS:= -m32 -ffreestanding -Wall -pedantic -Wextra -I. -fno-stack-protector -std=gnu99 -g
ASFLAGS:= --32
OBJ= bootloader/loader.o \
     kernel/stdio.o kernel/kernel.o kernel/tests.o kernel/hunter.o \
     lib/string.o \
     memory/memory.o memory/gdt.o memory/paging.o memory/heap.o \
       drivers/screen/screen.o drivers/screen/vga.o \
       drivers/keyboard/keyboard.o \
       drivers/timer/timer.o \
       drivers/sound/sound.o \
     interrupt/interrupt.o interrupt/interruptHandler.o interrupt/ldt.o \
     gui/gui.o \

LDFLAGS= -melf_i386 -T linker.ld
MK=Makefile.in

-include Makefile.in

all:tylyos.iso

.PHONY:version
version:
	@echo "REV" $@
	@echo -n "#pragma once\n#define REVISION " > version
	@svn info | grep Revision|head -1| grep -o "[0-9]*" >>  version

kernel/kernel.o:version
	@echo "GCC" $@
	@$(CC) $(CFLAGS) -c -o $@ kernel/kernel.c
	

tylyos.iso: tylyos Makefile.in
	@echo "Preparing iso..."
	@mkdir iso && \
	mkdir -p iso/boot/grub && \
	cp stage2_eltorito iso/boot/grub && \
	echo "timeout 0">>iso/boot/grub/menu.lst && \
	echo "title   TylyOS">>iso/boot/grub/menu.lst && \
	echo "kernel  /boot/tylyos">>iso/boot/grub/menu.lst && \
	echo "module /boot/c.img">>iso/boot/grub/menu.lst && \
	cp tylyos iso/boot/tylyos && \
	cp c.img iso/boot/c.img && \
	mkisofs -quiet -R -b boot/grub/stage2_eltorito -no-emul-boot \
	-boot-load-size 4 -boot-info-table -o $@ iso && \
	echo "Iso created" && \
	rm -R iso

Makefile.in:
	@echo "Configuring..."
	@./configure

tylyos: $(OBJ)
	@echo "LD" $@
	@$(LD) $(LDFLAGS) $^ -o $@

%.o:%.s 
	@echo "AS" $@
	@$(AS) $(ASFLAGS) $^ -o $@

%.o: %.c
	@echo "GCC" $@
	@$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.asm
	@echo "NASM" $@
	@$(NASM) -f elf -o $@ $^

.PHONY: clean
clean:
	rm -f *\~ */*\~ */*.o */*/*\~ */*/*.o log tylyos tylyos.iso tylyos.iso.sig

.PHONY: smallclean
smallclean:
	rm -f *\~ */*\~ */*.o */*/*\~ */*/*.o log tylyos tylyos.iso.sig

