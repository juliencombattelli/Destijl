/*
 * ThreadMove.cpp
 *
 *  Created on: 29 mai 2017
 *      Author: guill
 */

#include "../serial.h"
#include "../global.h"
#include <task.h>
#include <stdlib.h>

#include "ThreadWatchdog.h"

void threadMove(void* arg)
{
	printf("=> threadMove create\n");
	void *msg;

	if(rt_queue_bind(&queueMsgRobot,"queueMsgRobot",TM_INFINITE) != STATUS_OK)
		perror("bind queue error\n");

	printf("threadMove initialise\n");

	while(1)
	{
		rt_queue_receive(&queueMsgRobot, &msg,TM_INFINITE);
		printf("received message> bytes s=%s\n",(const char *)msg);
		if(strcmp((char*)msg, "C") == 0)
		{
			rt_queue_free(&queueMsgRobot,msg);
			break;
		}
		int etat = STATUS_ERR;

		//Zone critique
		if(rt_mutex_acquire(&mutexEtat, TM_INFINITE) != STATUS_OK)
			perror("erreur lors de la prise du mutexEtat\n");
		else
		{
			etat = etatCommRobot;
			if(rt_mutex_release(&mutexEtat) != STATUS_OK)
				perror("erreur lors de la liberation du mutexEtat\n");
		}
		//fin de zone critique

		if(etat == 0)
		{
			printf("send %s to robot\n", (char*)msg);
			int err = robotCmd(((char*)msg)[0]);

			if(rt_mutex_acquire(&mutexEtat, TM_INFINITE) != STATUS_OK)
				perror("erreur lors de la prise du mutexEtat\n");
			else
			{
				etatCommRobot = err;

				if(rt_mutex_release(&mutexEtat) != STATUS_OK)
					perror("erreur lors de la libération du mutexEtat\n");
			}
		}
		//recup err
		rt_queue_free(&queueMsgRobot,msg);
	}

	rt_queue_unbind(&queueMsgRobot);
	printf("=> threadMove kill\n");
}


