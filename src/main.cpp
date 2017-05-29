#if 0

#include "includes.h"
#include "global.h"
#include "fonctions.h"

using namespace std;
using namespace cv;
using namespace raspicam;

/**
 * \fn void initStruct(void)
 * \brief Initialisation des structures de l'application (t√¢ches, mutex, 
 * semaphore, etc.)
 */
void initStruct(void);

/**
 * \fn void startTasks(void)
 * \brief D√©marrage des t√¢ches
 */
void startTasks(void);

/**
 * \fn void deleteTasks(void)
 * \brief Arr√™t des t√¢ches
 */
void deleteTasks(void);

int main(int argc, char**argv) {
    printf("#################################\n");
    printf("#      DE STIJL PROJECT         #\n");
    printf("#################################\n");

   
    initStruct();
    printf("Init struct done\n");
    startTasks();
    printf("Start done\n");
    pause();
    /*deleteTasks();*/
    return 0;
}

void initStruct(void) {
    int err;
    /* Creation des mutex */
    if ((err = rt_mutex_create(&mutexEtat, "mutex_etat"))) {
        printf("Error mutex create : mutex_etat : %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_mutex_create(&mutexMove, "mutex_move"))) {
        printf("Error mutex create : mutex_move : %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    /* Creation du semaphore */
    if ((err = rt_sem_create(&semConnecterRobot, "sem_connect", 0, S_FIFO))) {
        
        printf("Error semaphore create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    
    /* Creation des taches */
    if ((err = rt_task_create(&tServeur, NULL, 0, PRIORITY_TSERVEUR, 0))) {
        printf("Error task create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    if ((err = rt_task_create(&tconnect, NULL, 0, PRIORITY_TCONNECT, 0))) {
        printf("Error task create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_task_create(&tmove, NULL, 0, PRIORITY_TMOVE, 0))) {
        printf("Error task create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_task_create(&tenvoyer, NULL, 0, PRIORITY_TENVOYER, 0))) {
        printf("Error task create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    /* Creation des files de messages */
    /*if (err = rt_queue_create(&queueMsgGUI, "toto", MSG_QUEUE_SIZE*sizeof(DMessage), MSG_QUEUE_SIZE, Q_FIFO)){
        printf("Error msg queue create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }*/

    /* Creation des structures globales du projet */
    /*robot = d_new_robot();
    move = d_new_movement();
    serveur = d_new_server();*/
}

void startTasks() {
    int err;
    if ((err = rt_task_start(&tconnect, &connecter, NULL))) {
        printf("Error task start: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_task_start(&tServeur, &communiquer, NULL))) {
        printf("Error task start: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_task_start(&tmove, &deplacer, NULL))) {
        printf("Error task start: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_task_start(&tenvoyer, &envoyer, NULL))) {
        printf("Error task start: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

}

void deleteTasks() {
    rt_task_delete(&tServeur);
    rt_task_delete(&tconnect);
    rt_task_delete(&tmove);
}

#endif

#include "imagerie.h"
#include "serial.h"
#include "tcpServer.h" // include himself imagerie.h
#include "global.h"
//#include <pthread.h>
#include <task.h>
#include <stdlib.h>

using namespace std;
using namespace cv;
using namespace raspicam;

#define START_MODE WITHOUT_WD

bool m_exit = false;

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
				rt_queue_send(&queueMsgGUI, msg_serv, 20, Q_NORMAL); // a vÈrifier

				void* msg_pos;

				if(!(msg_pos = rt_queue_alloc(&queueMsgGUI,20)))
					perror("alloc queue error\n");

				strcpy((char*)msg_pos, (char*)positionRobots);
				rt_queue_send(&queueMsgGUI, msg_pos, 20, Q_NORMAL); // a vÈrifier
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
				rt_queue_send(&queueMsgGUI, msg_serv, 20, Q_NORMAL); // a vÈrifier

				void* msg_img;

				if(!(msg_img = rt_queue_alloc(&queueMsgGUI,20)))
					perror("alloc queue error\n");

				strcpy((char*)msg_img, (char*)(&compress));
				rt_queue_send(&queueMsgGUI, msg_img, 20, Q_NORMAL); // a vÈrifier
			}
			//sendToUI("IMG",&compress);
			//envoie de l'image dans msgGUI
		}
		while(m_exit == false);
		closeCam(&rpiCam);

		if(rt_sem_v(&semConnecterServeur) != 0)
			perror("erreur lors de la libÈration du semConnecterServeur\n");
	}
	printf("=> threadVideo kill\n");
}

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
						rt_queue_send(&queueMsgGUI, msg, 20, Q_NORMAL); // a vÈrifier
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
						rt_queue_send(&queueMsgGUI, msg, 20, Q_NORMAL); // a vÈrifier
					}
				}
			}
			if(rt_mutex_release(&mutexEtat) != 0)
				perror("erreur lors de la libÈration du mutexEtat\n");
			//fin de zone critique
		}
	}

	robotCloseCom();

	printf("=> threadConnecter kill\n");
}

void threadEnvoyer(void *arg)
{
	printf("=> threadEnvoyer create\n");

	void *msg;

	if(rt_queue_bind(&queueMsgGUI,"queueMsgGUI",TM_INFINITE) != 0)
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

void threadServeur(void *arg)
{
	printf("=> threadServeur create\n");
	if((etatCommMoniteur = serverOpen()) == 0) // init serveur + ouvrir connexion serveur
	{
		char header[4];
		char data[20];
		memset(data, '\0',20);
		memset(header,'\0',4);

		if(rt_sem_v(&semConnecterServeur) != 0)
			perror("erreur lors de la libÈration du semConnecterServeur\n");

		do
		{
			receptionFromUI(header,data);
			if(strcmp(header, DMB) == 0)
			{
				printf("EVENEMENT DUMBER DETECTE AVEC LE MESSAGE :%s \n",data);

				if(data[0] == 'u' || data[0] == 'r')
				{
					if(rt_sem_v(&semConnecterRobot) != 0)
						perror("erreur lors de la liberation du semConnecterRobot\n");
				}
				else
				{
					printf("movement : %c\n", data[0]);
					void* msg_rbt;

					if(!(msg_rbt = rt_queue_alloc(&queueMsgRobot,20)))
						perror("alloc queue error\n");

					data[1] = '\0';
					strcpy((char*)msg_rbt,data);
					rt_queue_send(&queueMsgRobot, msg_rbt, 20, Q_NORMAL); // a vÈrifier
				}
			}

			if(strcmp(header, MES) == 0)
			{
				printf("EVENEMENT MESSAGE DETECTE AVEC LE MESSAGE :%s \n",data);

				if(data[0] == 'C')
				{
					void* msg_serv;
					void* msg_rbt;

					if(!(msg_serv = rt_queue_alloc(&queueMsgGUI,20)))
						perror("alloc queue error\n");
					if(!(msg_rbt = rt_queue_alloc(&queueMsgGUI,20)))
						perror("alloc queue error\n");

					strcpy((char*)msg_serv,"C");
					strcpy((char*)msg_rbt,"C");
					rt_queue_send(&queueMsgGUI, msg_serv, 20, Q_NORMAL); // a vÈrifier
					rt_queue_send(&queueMsgRobot, msg_rbt, 20, Q_NORMAL); // a vÈrifier
				}
			}

			if(strcmp(header,POS)==0)
			{
				printf("EVENEMENT POSITION DETECTE AVEC LE MESSAGE :%s \n",data);
			}
		}
		while((strcmp(header,MES)!=0) || (data[0] != 'C'));

		m_exit = true;

		if(rt_sem_p(&semConnecterServeur, TM_INFINITE) != 0)
			perror("erreur lors de la libÈration du semConnecterServeur\n");

		if(rt_sem_v(&semConnecterRobot) != 0)
			perror("erreur lors de la liberation du semConnecterRobot\n");

		serverClose();
	}
	printf("=> threadServeur kill\n");
}

void threadMove(void* arg)
{
	printf("=> threadMove create\n");
	void *msg;

	if(rt_queue_bind(&queueMsgRobot,"queueMsgRobot",TM_INFINITE) != 0)
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
		int etat = -1;

		//Zone critique
		if(rt_mutex_acquire(&mutexEtat, TM_INFINITE) != 0)
			perror("erreur lors de la prise du mutexEtat\n");
		else
		{
			etat = etatCommRobot;
			if(rt_mutex_release(&mutexEtat) != 0)
				perror("erreur lors de la liberation du mutexEtat\n");
		}
		//fin de zone critique

		if(etat == 0)
		{
			printf("send %s to robot\n", (char*)msg);
			int err = robotCmd(((char*)msg)[0]);

			if(rt_mutex_acquire(&mutexEtat, TM_INFINITE) != 0)
				perror("erreur lors de la prise du mutexEtat\n");
			else
			{
				etatCommRobot = err;

				if(rt_mutex_release(&mutexEtat) != 0)
					perror("erreur lors de la libÈration du mutexEtat\n");
			}
		}
		//recup err
		rt_queue_free(&queueMsgRobot,msg);
	}

	rt_queue_unbind(&queueMsgRobot);
	printf("=> threadMove kill\n");
}

void threadBatt(void* arg)
{
	printf("=> threadBatt create\n");
	if(rt_task_set_periodic(NULL, TM_NOW, rt_timer_ns2ticks(250000000)) != 0)
		perror("erreur lors du lancement du thread\n");
	else
	{
		while(m_exit == false)
		{
			rt_task_wait_period(NULL);

			int tmpEtat = -1;
			if(rt_mutex_acquire(&mutexEtat, TM_INFINITE) != 0)
				perror("erreur lors de la prise du mutexEtat\n");
			else
			{
				tmpEtat = etatCommRobot;

				if(rt_mutex_release(&mutexEtat) != 0)
					perror("erreur lors de la libÈration du mutexEtat\n");
			}

			if(tmpEtat == 0)
			{
				int vbat = robotCmd(GETVBAT);
				void* msg_serv;

				if(!(msg_serv = rt_queue_alloc(&queueMsgGUI,20)))
					perror("alloc queue error\n");

				strcpy((char*)msg_serv,BAT);
				rt_queue_send(&queueMsgGUI, msg_serv, 20, Q_NORMAL); // a vÈrifier

				void* msg_level;

				if(!(msg_level = rt_queue_alloc(&queueMsgGUI,20)))
					perror("alloc queue error\n");

				char buffer[2];
				sprintf (buffer, "%d", vbat);
				strcpy((char*)msg_level, buffer);
				rt_queue_send(&queueMsgGUI, msg_level, 20, Q_NORMAL); // a vÈrifier
			}
		}
	}

	printf("=> threadBatt kill\n");
}

void threadWatchdog(void* arg)
{
	printf("=> threadWatchdog create\n");
	if(rt_task_set_periodic(NULL, TM_NOW, rt_timer_ns2ticks(10*1000000)) != 0)
		perror("erreur lors du lancement du thread\n");
	else
	{
		while(m_exit == false)
		{
			rt_task_wait_period(NULL);

			int tmpEtat = -1;
			if(rt_mutex_acquire(&mutexEtat, TM_INFINITE) != 0)
				perror("erreur lors de la prise du mutexEtat\n");
			else
			{
				tmpEtat = etatCommRobot;

				if(rt_mutex_release(&mutexEtat) != 0)
					perror("erreur lors de la libÈration du mutexEtat\n");
			}

			printf("status : %d\r\n", tmpEtat);

			int status = robotCmd(RELOAD);

			if(rt_mutex_acquire(&mutexEtat, TM_INFINITE) != 0)
				perror("erreur lors de la prise du mutexEtat\n");
			else
			{
				etatCommRobot = status;

				if(rt_mutex_release(&mutexEtat) != 0)
					perror("erreur lors de la libÈration du mutexEtat\n");
			}
		}
	}

	printf("=> threadWatchdog kill\n");
}

int main()
{
	/*creation des semaphores*/
	if(rt_sem_create(&semConnecterServeur, "semConnecterServeur", 0, S_PRIO) != 0)
    	perror("erreur lors de la creation du sem ConnecterServeur\n");
    if(rt_sem_create(&semConnecterRobot, "semConnecterRobot", 0, S_PRIO) != 0)
        perror("erreur lors de la creation du sem sConnecterRobot\n");

    /*creation des mutex*/
    if(rt_mutex_create(&mutexEtat, "mutexEtat") != 0)
    	perror("erreur lors de la creation du mutex mutexEtat\n");
    if(rt_mutex_create(&mutexMove, "mutexMove") != 0)
       	perror("erreur lors de la creation du mutex mutexMove\n");

    /*creation des queues*/
    if(rt_queue_create(&queueMsgGUI, "queueMsgGUI", 30, Q_UNLIMITED, Q_PRIO) != 0)
    	perror("erreur lors de la creation de la queue queueMsgGUI\n");
    if(rt_queue_create(&queueMsgRobot, "queueMsgRobot", 20, Q_UNLIMITED, Q_PRIO) != 0)
        perror("erreur lors de la creation de la queue queueMsgRobot\n");

    /*creation des taches*/
	if(rt_task_spawn(&tServeur, "serveur", 0, 30, T_JOINABLE, &threadServeur, NULL) != 0)
		perror("erreur lors de la creation du thread Serveur\n");
	if(rt_task_spawn(&tvideo,"tvideo", 0, 43, T_JOINABLE, &threadVideo, NULL) != 0)
		perror("erreur lors de la creation du thread Video\n");
	if(rt_task_spawn(&tconnect,"tconnect", 0, 35, T_JOINABLE, &threadConnecter, NULL) != 0)
		perror("erreur lors de la creation du thread Video\n");
	if(rt_task_spawn(&tenvoyer,"tenvoyer", 0, 44, T_JOINABLE, &threadEnvoyer, NULL) != 0)
		perror("erreur lors de la creation du thread envoyer\n");
	if(rt_task_spawn(&tmove, "tmove", 0, 45, T_JOINABLE, &threadMove, NULL) != 0)
		perror("erreur lors de la creation du thread move\n");
	if(rt_task_spawn(&tbattery, "tbattery", 0, 34, T_JOINABLE, &threadBatt, NULL) != 0)
		perror("erreur lors de la creation du thread batt\n");
	if(START_MODE == WITH_WD)
		if(rt_task_spawn(&tWatchdog, "tWatchdog", 0, 99, T_JOINABLE, &threadWatchdog, NULL) != 0)
			perror("erreur lors de la creation du thread wdg\n");

    rt_task_join(&tServeur);
    rt_task_join(&tvideo);
    rt_task_join(&tconnect);
    rt_task_join(&tenvoyer);
    rt_task_join(&tmove);
    rt_task_join(&tbattery);

    /*destruction des taches*/
    rt_task_delete(&tServeur);
    rt_task_delete(&tvideo);
    rt_task_delete(&tconnect);
    rt_task_delete(&tenvoyer);
    rt_task_delete(&tmove);
    rt_task_delete(&tbattery);
    rt_task_delete(&tWatchdog);

    /*destruction des mutex*/
    rt_mutex_delete(&mutexEtat);
    rt_mutex_delete(&mutexMove);

    /*destruction des semaphores*/
    rt_sem_delete(&semConnecterServeur);
    rt_sem_delete(&semConnecterRobot);

    /*destruction de la queue*/
    rt_queue_delete(&queueMsgGUI);
    rt_queue_delete(&queueMsgRobot);

    printf("end main \n");
    return 0;
}

