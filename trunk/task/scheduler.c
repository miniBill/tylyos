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
    scheduleTimeSlice=200;
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

void forceSchedule()
{
    lastSchedule=time()-scheduleTimeSlice-1;
    schedule();
}


/*funzione che sceglie il task da mandare in esecuzione e che lo manda in esecuzione usando il dispatcher*/
void schedule()
{
    if(time()-lastSchedule>=scheduleTimeSlice)
    {
        if(activeScheduler==0)
            return;

        lastSchedule=time();
        unsigned int id;
        struct taskListElement *pointer=taskListRoot;

            unsigned long int min=0xFFFFFFFF;
            id=0;

        if ( pointer!=0 )
        {
            do
            {
                /*controlla se il task in sllep deve essere risvegliato*/
                if (
                     pointer->task->stato==TASK_STATE_SLEEPING &&
                     pointer->task->statoInfo<time()
                   )
                {
                    pointer->task->stato=TASK_STATE_READY;
                }
                /*se il task e' pronto ed e' quello eseguito meno recentemente*/
                if (
                     pointer->task->stato==TASK_STATE_READY &&
                     pointer->task->lastScheduledTime<min
                   )
                {
                    min=pointer->task->lastScheduledTime;
                    id=pointer->task->procID;
                }
                pointer=pointer->next;
            }
            while ( pointer!=0 );
        }
        if(id>0)
        {
           // printf(1,"<scheduler in action> %s\n",getTask(id)->nome);
            dispatch(id);
        }
        else
        {
            idle();
        }

    }
}
/*
funzioneche viene richimata quando nessun task ha niente da fare
dovra' mandare in esecuzione un task dedicato alla gestione dei tempi morti
*/
void idle()
{
//    printf(1,"idle\n");
    dispatch(0);
}
