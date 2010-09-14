/* Copyright (C) 2008 Luca Salmin
 * Copyright (C) 2008 Leonardo Taglialegne <leonardotaglialegne+tylyos@gmail.com>
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

#ifndef PIPE_H
#define PIPE_H

#define PIPE_BUFFER_SIZE 50

struct pipe
{
    int inputPointer;
    int outputPointer;
    char buffer[PIPE_BUFFER_SIZE];
};

/*alloca una pipe e ne ritorna la struct*/
struct pipe *createPipe();
/*dealloca la struttura*/
void deletePipe(struct pipe *p);

/*ritorna il numero di byte scritti, se e' minore di count significa che la pipe e' piena*/
unsigned int writeOnPipe(struct pipe *p,char *data,unsigned int count);
/*ritorna il numero di byte letti, se e' minore di count significa che la pipe e' vuota*/
unsigned int readOnPipe(struct pipe *p,char *data,unsigned int count);

#endif
