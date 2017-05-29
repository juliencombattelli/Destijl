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

	while(m_exit == false)
	{
		if(rt_sem_p(&semConnecterRobot, TM_INFINITE) != 0)
			perror("erreur lors de la prise du semConnecterRobot\n");

		printf("debloquage du semaphore semConnecterRobot\n");

		if(m_exit == false)
		{
			//Zone critique
			if(rt_mutex_acquire(&mutexEtat, TM_INFINITE) != 0)
				perror("erreur lors de la prise du mutexEtat\n");
			else
			{
				if(etatCommRobot != 0)
				{
					etatCommRobot = robotCmd(START_MODE);
					if(etatCommRobot == 0)
					{
						if(!(msg = rt_queue_alloc(&queueMsgGUI,20)))
							perror("alloc queue error\n");

						strcpy((char*)msg,ACK);
						rt_queue_send(&queueMsgGUI, msg, 20, Q_NORMAL); // a vérifier
					}
				}
				else
				{
					if(robotCmd('r') == 0)
					{
						etatCommRobot = -1;

						if(!(msg = rt_queue_alloc(&queueMsgGUI,20)))
							perror("alloc queue error\n");

						strcpy((char*)msg,ACK);
						rt_queue_send(&queueMsgGUI, msg, 20, Q_NORMAL); // a vérifier
					}
				}
			}
			if(rt_mutex_release(&mutexEtat) != 0)
				perror("erreur lors de la libération du mutexEtat\n");
			//fin de zone critique
		}
	}

	robotCloseCom();

	printf("=> threadConnecter kill\n");
}


