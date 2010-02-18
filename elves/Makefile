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

CFLAGS:= -march=i386 -ffreestanding -Wall -pedantic -Wextra -I ../trunk -fno-stack-protector -std=gnu99 -g -Os
ASFLAGS:= -march=i386

MK=Makefile.in

-include Makefile.in

Makefile.in:
	@echo "Configuring..."
	@./configure

%.o:%.s 
	@echo "AS" $@
	@$(AS) $(ASFLAGS) $^ -o $@

%.o: %.c
	@echo "GCC" $@
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.asm
	@echo "NASM" $@
	@nasm -f elf -o $@ $^

.PHONY: clean
clean:
	rm -f *\~ *.o

