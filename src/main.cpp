
#include "includes.h"
#include "global.h"
#include "fonctions.h"

using namespace std;
using namespace cv;
using namespace raspicam;

/**
 * \fn void initStruct(void)
 * \brief Initialisation des structures de l'application (tâches, mutex, 
 * semaphore, etc.)
 */
void initStruct(void);

/**
 * \fn void startTasks(void)
 * \brief Démarrage des tâches
 */
void startTasks(void);

/**
 * \fn void deleteTasks(void)
 * \brief Arrêt des tâches
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
    if ((err = rt_mutex_create(&mutexEtat, NULL))) {
        printf("Error mutex create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }
    if ((err = rt_mutex_create(&mutexMove, NULL))) {
        printf("Error mutex create: %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    /* Creation du semaphore */
    if ((err = rt_sem_create(&semConnecterRobot, NULL, 0, S_FIFO))) {
        
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
