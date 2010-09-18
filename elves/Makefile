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

CFLAGS:= -m32 -ffreestanding -Wall -pedantic -Wextra -I ../kernel -fno-stack-protector -std=gnu99 -nostdlib
LDFLAGS:= -nostdlib -melf_i386 -T linker.ld
ASFLAGS:= --32
PROGS:=ottanta hello idle init shell

.PHONY:all
all:$(PROGS)
	cp $(PROGS) ../fsMaker/root/

shell:shell.o
	$(LD) $(LDFLAGS) -o $@ $^
	strip $@

init:init.o
	$(LD) $(LDFLAGS) -o $@ $^
	strip $@

ottanta:ottanta.o
	$(LD) $(LDFLAGS) -o $@ $^
	strip $@

hello:hello.o
	$(LD) $(LDFLAGS) -o $@ $^
	strip $@

idle:idle.o
	$(LD) $(LDFLAGS) -o $@ $^
	strip $@

.PHONY: clean
clean:
	rm -f *\~ *.o ottanta hello idle init shell

.PHONY: copy
copy:
	cp $(PROGS) ../fsMaker/root
