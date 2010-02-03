/* Copyright (C) 2010 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
* Copyright (C) 2010 Luca Salmin
*
* This file is part of TylyOS.
*
* TylyOS is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* TylyOS is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with TylyOS.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "hunter.h"
#include <drivers/screen/screen.h>
#include <lib/string.h>

int hunt(multiboot_info_t * info){
  if(info->mods_count==0){ //no modules, something is wrong, so use write
    write("niente\n",0);
    return 0;
  }
  module_t * mod=(module_t*)info->mods_addr; //structure containing first module info
    char * m=(char*)mod->mod_start; //get a pointer to the actual module
  printf(0,"%s caricato a 0x%x [%s]\n",mod->string,mod->mod_start,m);
  return 1;
}