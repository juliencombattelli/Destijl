/*
 * ThreadEnvoyer.cpp
 *
 *  Created on: 29 mai 2017
 *      Author: guill
 */

#include "../serial.h"
#include "../global.h"
#include <task.h>
#include <stdlib.h>

#include "ThreadWatchdog.h"

void threadEnvoyer(void *arg)
{
	printf("=> threadEnvoyer create\n");

	void *msg;

	if(rt_queue_bind(&queueMsgGUI,"queueMsgGUI",TM_INFINITE) != STATUS_OK)
		perror("bind queue error\n");

	printf("threadEnvoyer initialise\n");

	while(1)
	{
		rt_queue_receive(&queueMsgGUI, &msg,TM_INFINITE);
		printf("received message GUI> bytes s=%s\n",(const char *)msg);
		if(strcmp((char*)msg, "C") == 0)
		{
			rt_queue_free(&queueMsgGUI,msg);
			break;
		}
		else if(strcmp((char*)msg, BAT) == 0)
		{
			rt_queue_free(&queueMsgGUI,msg);
			rt_queue_receive(&queueMsgGUI, &msg,TM_INFINITE);
			sendToUI(BAT, msg);
		}
		else if(strcmp((char*)msg, POS) == 0)
		{
			rt_queue_free(&queueMsgGUI,msg);
			rt_queue_receive(&queueMsgGUI, &msg,TM_INFINITE);
			sendToUI(POS, msg); // a corriger
		}
		else if(strcmp((char*)msg, IMG) == 0)
		{
			rt_queue_free(&queueMsgGUI,msg);
			rt_queue_receive(&queueMsgGUI, &msg,TM_INFINITE);
			sendToUI(IMG, msg);
		}
		else
			sendToUI((char*)msg);

		rt_queue_free(&queueMsgGUI,msg);
	}

	rt_queue_unbind(&queueMsgGUI);
	printf("=> threadEnvoyer kill\n");
}


