/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
 * Copyright (C) 2008 Roberto Domenella
 *
 * This file is part of ClearOS.
 *
 * ClearOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ClearOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ClearOS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdio.h"

int strapp(char* dest,char* format,void* p){
	/*indice format string*/
	int i=0;
	/*indice output*/
	int o=0;
	while(dest[o]!=0) o++;
	for(;format[i]!=0;i++){
		if(format[i]=='%'){
			int base=10;
			int num=(int)p;
			char buf[MAXN] = {0};
			/*lunghezza del numero*/
			int l;
			/*indice per copiare*/
			int b;
			switch(format[i+1]){
				case 'd':
					break;
				case 'x':
					base=16;
					break;
				case 'b':
					base=2;
					break;
				case 'o':
					base=8;
					break;
				default:
					/*NON DOVREBBE SUCCEDERE, E SE SUCCEDE LA FORMAT STRING E' SBAGLIATA*/
					base=10;
					break;
			}
			for(l=0;num && (l<MAXN) ; num /= base,l++)
				buf[l] = "0123456789ABCDEF"[num % base];
			for(b=(l-1);b>=0;b--)
				dest[o++]=buf[b];
			i++;
		}
		else
			dest[o++]=format[i];
	}
	dest[o]=0;
	return 0;
}
