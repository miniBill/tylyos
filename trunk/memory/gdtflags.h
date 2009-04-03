/* Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+clearos@gmail.com>
 * Copyright (C) 2008 Luca Salmin
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

#ifndef GDTFLAGS_H_
#define GDTFLAGS_H_


#define RPL_USER 0x3
#define RPL_KERNEL 0x0

enum{
                            /*         V                    */
    MEM_RO          = 0x00, /*00|00|00|00 0=read only       */
    MEM_RW          = 0x02, /*00|00|00|10 1=read and write  */

                            /*    V                         */
    MEM_SYS         = 0x00, /*00|00|00|00 0=sistema         */
    MEM_CODE_DATA   = 0x10, /*00|01|00|00 1=codice o dati   */

                            /*      V                       */
    MEM_DATA        = 0x00, /*00|00|00|00 0=dati            */
    MEM_CODE        = 0x08, /*00|00|10|00 1=codice          */


    MEM_TSS        = 0x09, /*00|00|10|01           */
    MEM_TSS_BUSY   = 0x0B, /*00|00|10|11           */

                            /* V V                          */
    MEM_KERNEL      = 0x00, /*00|00|00|00 00=ring0 (kernel) */
    MEM_RING1       = 0x20, /*00|10|00|00 01=ring1          */
    MEM_RING2       = 0x40, /*01|00|00|00 10=ring2          */
    MEM_USER        = 0x60, /*01|10|00|00 11=ring3 (user)   */

                            /*V                             */
    MEM_NOT_PRESENT = 0x00, /*00|00|00|00 0=non presente    */
    MEM_PRESENT     = 0x80  /*10|00|00|00 1=presente        */
};

enum{
    MEM_16          = 0x00, /*00|00|00|00 segmento a 16bit  */
    MEM_32          = 0x40, /*01|00|00|00 segmento a 32bit  */

    MEM_FINE        = 0x00, /*00|00|00|00 0=1byte           */
    MEM_GRANULAR    = 0x80  /*10|00|00|00 1=4Kbyte          */
};
#endif
