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

#ifndef SCHEDULER_H
#define SCHEDULER_H

unsigned int idleTaskID;/*contiene l'id del task idle in modo che lo scheduler si accorga quando arriva il momento di eseguire le operazioni*/

unsigned int scheduleTimeSlice;
unsigned long int lastSchedule;
char activeScheduler;
/*inizializza le strutture che servono allo cheduler per funzionare*/
void initScheduler();

/*setta ogni quanti millisecondi viene eseguito l' algoritmo di scheduling*/
void setSchedulerTimeSlice(unsigned int ms);

/*abilita lo scheduler*/
void startScheduler();
/*disabilita lo scheduling dei task*/
void stopScheduler();

void forceSchedule();

/*funzione che sceglie il task da mandare in esecuzione e che lo manda in esecuzione usando il dispatcher*/
void schedule();
/*
funzioneche viene richimata quando nessun task ha niente da fare
dovra' mandare in esecuzione un task dedicato alla gestione dei tempi morti
*/
void idle();

#endif
