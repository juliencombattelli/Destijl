/*
 * ThreadConnecter.cpp
 *
 *  Created on: 29 mai 2017
 *      Author: guill
 */

#include "../serial.h"
#include "../global.h"
#include <task.h>
#include <stdlib.h>

#include "ThreadConnecter.h"

void threadConnecter(void *arg)
{
	printf("=> threadConnecter create\n");
	robotOpenCom();

	void *msg;

	while(exitApplication == false)
	{
		if(rt_sem_p(&semConnecterRobot, TM_INFINITE) != STATUS_OK)
			perror("erreur lors de la prise du semConnecterRobot\n");

		printf("debloquage du semaphore semConnecterRobot\n");

		if(exitApplication == false)
		{
			//Zone critique
			if(rt_mutex_acquire(&mutexEtat, TM_INFINITE) != STATUS_OK)
				perror("erreur lors de la prise du mutexEtat\n");
			else
			{
				if(etatCommRobot != STATUS_OK)
				{
					etatCommRobot = robotCmd(START_MODE);
					if(etatCommRobot == STATUS_OK)
					{
						if(!(msg = rt_queue_alloc(&queueMsgGUI,MSG_QUEUE_SIZE)))
							perror("alloc queue error\n");

						strcpy((char*)msg,ACK);
						rt_queue_send(&queueMsgGUI, msg, MSG_QUEUE_SIZE, Q_NORMAL); // a vérifier
					}
				}
				else
				{
					if(robotCmd('r') == STATUS_OK)
					{
						etatCommRobot = STATUS_ERR;

						if(!(msg = rt_queue_alloc(&queueMsgGUI,MSG_QUEUE_SIZE)))
							perror("alloc queue error\n");

						strcpy((char*)msg,ACK);
						rt_queue_send(&queueMsgGUI, msg, MSG_QUEUE_SIZE, Q_NORMAL); // a vérifier
					}
				}
			}
			if(rt_mutex_release(&mutexEtat) != STATUS_OK)
				perror("erreur lors de la libération du mutexEtat\n");
			//fin de zone critique
		}
	}

	robotCloseCom();

	printf("=> threadConnecter kill\n");
}


