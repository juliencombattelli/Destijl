#include "imagerie.h"
#include "serial.h"
#include "tcpServer.h" // include himself imagerie.h
#include "global.h"
#include <task.h>
#include <stdlib.h>

#include "thread/ThreadWatchdog.h"
#include "thread/ThreadVideo.h"
#include "thread/ThreadConnecter.h"
#include "thread/ThreadEnvoyer.h"
#include "thread/ThreadServeur.h"
#include "thread/ThreadMove.h"
#include "thread/ThreadBatt.h"

using namespace std;
using namespace cv;
using namespace raspicam;

void creatSemephores()
{
	if(rt_sem_create(&semConnecterServeur, "semConnecterServeur", 0, S_PRIO) != 0)
		perror("erreur lors de la creation du sem ConnecterServeur\n");
	if(rt_sem_create(&semConnecterRobot, "semConnecterRobot", 0, S_PRIO) != 0)
		perror("erreur lors de la creation du sem sConnecterRobot\n");
}

void creatMutex()
{
	if(rt_mutex_create(&mutexEtat, "mutexEtat") != 0)
		perror("erreur lors de la creation du mutex mutexEtat\n");
	if(rt_mutex_create(&mutexMove, "mutexMove") != 0)
		perror("erreur lors de la creation du mutex mutexMove\n");
}

void creatQueues()
{
	if(rt_queue_create(&queueMsgGUI, "queueMsgGUI", 30, Q_UNLIMITED, Q_PRIO) != 0)
		perror("erreur lors de la creation de la queue queueMsgGUI\n");
	if(rt_queue_create(&queueMsgRobot, "queueMsgRobot", 20, Q_UNLIMITED, Q_PRIO) != 0)
		perror("erreur lors de la creation de la queue queueMsgRobot\n");
}

void spawnTasks()
{
	if(rt_task_spawn(&tServeur, "serveur", 0, PRIORITY_TSERVEUR, T_JOINABLE, &threadServeur, NULL) != 0)
		perror("erreur lors de la creation du thread Serveur\n");
	if(rt_task_spawn(&tvideo,"tvideo", 0, PRIORITY_TVIDEO, T_JOINABLE, &threadVideo, NULL) != 0)
		perror("erreur lors de la creation du thread Video\n");
	if(rt_task_spawn(&tconnect,"tconnect", 0, PRIORITY_TCONNECT, T_JOINABLE, &threadConnecter, NULL) != 0)
		perror("erreur lors de la creation du thread Video\n");
	if(rt_task_spawn(&tenvoyer,"tenvoyer", 0, PRIORITY_TENVOYER, T_JOINABLE, &threadEnvoyer, NULL) != 0)
		perror("erreur lors de la creation du thread envoyer\n");
	if(rt_task_spawn(&tmove, "tmove", 0, PRIORITY_TMOVE, T_JOINABLE, &threadMove, NULL) != 0)
		perror("erreur lors de la creation du thread move\n");
	if(rt_task_spawn(&tbattery, "tbattery", 0, PRIORITY_TBATTERY, T_JOINABLE, &threadBatt, NULL) != 0)
		perror("erreur lors de la creation du thread batt\n");
	if(START_MODE == WITH_WD)
		if(rt_task_spawn(&tWatchdog, "tWatchdog", 0, PRIORITY_TWATCHDOG, T_JOINABLE, &threadWatchdog, NULL) != 0)
			perror("erreur lors de la creation du thread wdg\n");
}

void joinTasks()
{
	rt_task_join(&tServeur);
	rt_task_join(&tvideo);
	rt_task_join(&tconnect);
	rt_task_join(&tenvoyer);
	rt_task_join(&tmove);
	rt_task_join(&tbattery);
}

void deleteTasks()
{
	rt_task_delete(&tServeur);
	rt_task_delete(&tvideo);
	rt_task_delete(&tconnect);
	rt_task_delete(&tenvoyer);
	rt_task_delete(&tmove);
	rt_task_delete(&tbattery);
	rt_task_delete(&tWatchdog);
}

void deleteMutex()
{
	rt_mutex_delete(&mutexEtat);
	rt_mutex_delete(&mutexMove);
}

void deleteSemaphores()
{
	rt_sem_delete(&semConnecterServeur);
	rt_sem_delete(&semConnecterRobot);
}

void deleteQueues()
{
	rt_queue_delete(&queueMsgGUI);
	rt_queue_delete(&queueMsgRobot);
}

int main()
{
	creatSemephores();
	creatMutex();
	creatQueues();

	spawnTasks();

	joinTasks();

	deleteTasks();

	deleteMutex();
	deleteSemaphores();
	deleteQueues();

    printf("end main \n");
    return 0;
}

