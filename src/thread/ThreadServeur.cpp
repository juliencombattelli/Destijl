/*
 * ThreadServeur.cpp
 *
 *  Created on: 29 mai 2017
 *      Author: guill
 */

#include "../serial.h"
#include "../global.h"
#include "../tcpServer.h" // include himself imagerie.h
#include <task.h>
#include <stdlib.h>

#include "ThreadWatchdog.h"

void threadServeur(void *arg)
{
	printf("=> threadServeur create\n");
	if((etatCommMoniteur = serverOpen()) == STATUS_OK) // init serveur + ouvrir connexion serveur
	{
		char header[4];
		char data[20];
		memset(data, '\0',20);
		memset(header,'\0',4);

		if(rt_sem_v(&semConnecterServeur) != STATUS_OK)
			perror("erreur lors de la libération du semConnecterServeur\n");

		do
		{
			receptionFromUI(header,data);
			if(strcmp(header, DMB) == 0)
			{
				printf("EVENEMENT DUMBER DETECTE AVEC LE MESSAGE :%s \n",data);

				if(data[0] == 'u' || data[0] == 'r')
				{
					if(rt_sem_v(&semConnecterRobot) != STATUS_OK)
						perror("erreur lors de la liberation du semConnecterRobot\n");
				}
				else
				{
					printf("movement : %c\n", data[0]);
					void* msg_rbt;

					if(!(msg_rbt = rt_queue_alloc(&queueMsgRobot,MSG_QUEUE_SIZE)))
						perror("alloc queue error\n");

					data[1] = '\0';
					strcpy((char*)msg_rbt,data);
					rt_queue_send(&queueMsgRobot, msg_rbt, MSG_QUEUE_SIZE, Q_NORMAL); // a vérifier
				}
			}

			if(strcmp(header, MES) == 0)
			{
				printf("EVENEMENT MESSAGE DETECTE AVEC LE MESSAGE :%s \n",data);

				if(data[0] == CLOSE)
				{
					void* msg_serv;
					void* msg_rbt;

					if(!(msg_serv = rt_queue_alloc(&queueMsgGUI,MSG_QUEUE_SIZE)))
						perror("alloc queue error\n");
					if(!(msg_rbt = rt_queue_alloc(&queueMsgGUI,MSG_QUEUE_SIZE)))
						perror("alloc queue error\n");

					strcpy((char*)msg_serv,"C");
					strcpy((char*)msg_rbt,"C");
					rt_queue_send(&queueMsgGUI, msg_serv, MSG_QUEUE_SIZE, Q_NORMAL); // a vérifier
					rt_queue_send(&queueMsgRobot, msg_rbt, MSG_QUEUE_SIZE, Q_NORMAL); // a vérifier
				}
			}

			if(strcmp(header,POS)==0)
			{
				printf("EVENEMENT POSITION DETECTE AVEC LE MESSAGE :%s \n",data);
			}
		}
		while((strcmp(header,MES)!=0) || (data[0] != 'C'));

		exitApplication = true;

		if(rt_sem_p(&semConnecterServeur, TM_INFINITE) != STATUS_OK)
			perror("erreur lors de la libération du semConnecteServeur\n");

		if(rt_sem_v(&semConnecterRobot) != 0)
			perror("erreur lors de la liberation du semConnecterRobot\n");

		serverClose();
	}
	printf("=> threadServeur kill\n");
}
