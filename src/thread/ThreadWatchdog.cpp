/*
 * ThreadWatchdog.cpp
 *
 *  Created on: 29 mai 2017
 *      Author: guill
 */

#include "../serial.h"
#include "../global.h"
#include <task.h>
#include <stdlib.h>

#include "ThreadWatchdog.h"

void threadWatchdog(void* arg)
{
	printf("=> threadWatchdog create\n");
	if(rt_task_set_periodic(NULL, TM_NOW, rt_timer_ns2ticks(10*1000000)) != 0)
		perror("erreur lors du lancement du thread\n");
	else
	{
		while(exitApplication == false)
		{
			rt_task_wait_period(NULL);

			int tmpEtat = STATUS_ERR;
			if(rt_mutex_acquire(&mutexEtat, TM_INFINITE) != STATUS_OK)
				perror("erreur lors de la prise du mutexEtat\n");
			else
			{
				tmpEtat = etatCommRobot;

				if(rt_mutex_release(&mutexEtat) != STATUS_OK)
					perror("erreur lors de la libération du mutexEtat\n");
			}

			printf("status : %d\r\n", tmpEtat);

			int status = robotCmd(RELOAD);

			if(rt_mutex_acquire(&mutexEtat, TM_INFINITE) != STATUS_OK)
				perror("erreur lors de la prise du mutexEtat\n");
			else
			{
				etatCommRobot = status;

				if(rt_mutex_release(&mutexEtat) != STATUS_OK)
					perror("erreur lors de la libération du mutexEtat\n");
			}
		}
	}

	printf("=> threadWatchdog kill\n");
}


