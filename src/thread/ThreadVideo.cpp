/*
 * ThreadVideo.cpp
 *
 *  Created on: 29 mai 2017
 *      Author: guill
 */

#include "../serial.h"
#include "../global.h"
#include <task.h>
#include <stdlib.h>

#include "ThreadVideo.h"

void threadVideo(void *arg)
{
	printf("=> threadVideo created \n");

	if(rt_sem_p(&semConnecterServeur, TM_INFINITE) != 0)
		perror("erreur lors de la prise du semConnecterServeur\n");

	if(rt_task_set_periodic(NULL, TM_NOW, rt_timer_ns2ticks(500000000)) != 0)
	    perror("erreur lors du lancement du thread\n");
	else
	{
		printf("threadVideo lance \n");
		Camera rpiCam;
		Image imgVideo;
		Arene monArene;
		position positionRobots[20];
		Jpg compress;

		openCamera(&rpiCam);
		do
		{
			rt_task_wait_period(NULL);

			getImg(&rpiCam, &imgVideo);
			if(detectArena(&imgVideo, &monArene)==0)
			{
				detectPosition(&imgVideo,positionRobots,&monArene);
				drawArena(&imgVideo,&imgVideo,&monArene);
			}
			else
				detectPosition(&imgVideo,positionRobots);

			//envoie position dans msgGui
			{
				void* msg_serv;
				if(!(msg_serv = rt_queue_alloc(&queueMsgGUI,20)))
					perror("alloc queue error\n");

				strcpy((char*)msg_serv,POS);
				rt_queue_send(&queueMsgGUI, msg_serv, 20, Q_NORMAL); // a vérifier

				void* msg_pos;

				if(!(msg_pos = rt_queue_alloc(&queueMsgGUI,20)))
					perror("alloc queue error\n");

				strcpy((char*)msg_pos, (char*)positionRobots);
				rt_queue_send(&queueMsgGUI, msg_pos, 20, Q_NORMAL); // a vérifier
			}
			//fin envoie position dans msgGui

			drawPosition(&imgVideo,&imgVideo,&positionRobots[0]);
			imgCompress(&imgVideo,&compress);

			//envoie de l'image dans msgGUI
			{
				void* msg_serv;
				if(!(msg_serv = rt_queue_alloc(&queueMsgGUI,20)))
					perror("alloc queue error\n");

				strcpy((char*)msg_serv,IMG);
				rt_queue_send(&queueMsgGUI, msg_serv, 20, Q_NORMAL); // a vérifier

				void* msg_img;

				if(!(msg_img = rt_queue_alloc(&queueMsgGUI,20)))
					perror("alloc queue error\n");

				strcpy((char*)msg_img, (char*)(&compress));
				rt_queue_send(&queueMsgGUI, msg_img, 20, Q_NORMAL); // a vérifier
			}
			//sendToUI("IMG",&compress);
			//envoie de l'image dans msgGUI
		}
		while(m_exit == false);
		closeCam(&rpiCam);

		if(rt_sem_v(&semConnecterServeur) != 0)
			perror("erreur lors de la libération du semConnecterServeur\n");
	}
	printf("=> threadVideo kill\n");
}



