/* Copyright (C) 2008 Luca Salmin
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

#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <task/task.h>

/*il tsk che sta' usando la cpu attualmente*/
struct taskStruct *runningTask;

void loadTSSregister(short unsigned int selector);
unsigned int getTSS();

void dispatch(int procID);
void dispatcher_mapPages(struct taskStruct *t);

#endif
