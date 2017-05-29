/*
 * ThreadBatt.cpp
 *
 *  Created on: 29 mai 2017
 *      Author: guill
 */

#include "../serial.h"
#include "../global.h"
#include <task.h>
#include <stdlib.h>

#include "ThreadWatchdog.h"

void threadBatt(void* arg)
{
	printf("=> threadBatt create\n");
	if(rt_task_set_periodic(NULL, TM_NOW, rt_timer_ns2ticks(250000000)) != STATUS_OK)
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

			if(tmpEtat == STATUS_OK)
			{
				int vbat = robotCmd(GETVBAT);
				void* msg_serv;

				if(!(msg_serv = rt_queue_alloc(&queueMsgGUI,MSG_QUEUE_SIZE)))
					perror("alloc queue error\n");

				strcpy((char*)msg_serv,BAT);
				rt_queue_send(&queueMsgGUI, msg_serv, MSG_QUEUE_SIZE, Q_NORMAL); // a vérifier

				void* msg_level;

				if(!(msg_level = rt_queue_alloc(&queueMsgGUI,MSG_QUEUE_SIZE)))
					perror("alloc queue error\n");

				char buffer[2];
				sprintf (buffer, "%d", vbat);
				strcpy((char*)msg_level, buffer);
				rt_queue_send(&queueMsgGUI, msg_level, MSG_QUEUE_SIZE, Q_NORMAL); // a vérifier
			}
		}
	}

	printf("=> threadBatt kill\n");
}


