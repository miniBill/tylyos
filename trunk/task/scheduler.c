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

#include "scheduler.h"
#include <task/dispatcher.h>
#include <drivers/timer/timer.h>

void initScheduler()
{
    scheduleTimeSlice=5000;
    activeScheduler=0;
    lastSchedule=0;
}

/*setta ogni quanti millisecondi viene eseguito l' algoritmo di scheduling*/
void setSchedulerTimeSlice(unsigned int ms)
{
    scheduleTimeSlice=ms;
}

/*abilita lo scheduler*/
void startScheduler()
{
    activeScheduler=1; 
}

/*disabilita lo scheduling dei task*/
void stopScheduler()
{
    activeScheduler=0; 
}

/*funzione che sceglie il task da mandare in esecuzione e che lo manda in esecuzione usando il dispatcher*/
void schedule()
{
    if(activeScheduler==0)
        return;
    if(time()-lastSchedule>=scheduleTimeSlice)
    {
        printf(0,"<scheduler in action>\n");
        lastSchedule=time();
        dispatch(0);
        /*TODO: scrivere codice che sceglie il task e lo manda in esecuzione*/
    }
}
/*
funzioneche viene richimata quando nessun task ha niente da fare
dovra' mandare in esecuzione un task dedicato alla gestione dei tempi morti
*/
void idle()
{
}
