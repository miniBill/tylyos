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

export GRUBIMAGESDIR=/lib/grub/i386-pc

mkdir iso
mkdir -p iso/boot/grub
cp $GRUBIMAGESDIR/stage2_eltorito iso/boot/grub

echo "timeout 1">>iso/boot/grub/menu.lst
echo "title   ClearOS">>iso/boot/grub/menu.lst
#echo "root    (hd0,0)">>iso/boot/grub/menu.lst
echo "kernel  /boot/clearos">>iso/boot/grub/menu.lst

cp clearos iso/boot/clearos

mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o iso.img iso

rm -R iso
