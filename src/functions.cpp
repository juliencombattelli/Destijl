/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "fonctions.h"

int write_in_queue(RT_QUEUE *msgQueue, void * data, int size);

void envoyer(void * arg) {
    printf("tenvoyer : demarrage\n");
    /*   DMessage *msg;
       int err;

       while (1) {
           rt_printf("tenvoyer : Attente d'un message\n");
           if ((err = rt_queue_read(&queueMsgGUI, &msg, sizeof (DMessage), TM_INFINITE)) >= 0) {
               rt_printf("tenvoyer : envoi d'un message au moniteur\n");
               serveur->send(serveur, msg);
               msg->free(msg);
           } else {
               rt_printf("Error msg queue write: %s\n", strerror(-err));
           }
       }*/
}

void connecter(void * arg) {
    printf("tconnecter : demarrage\n");
    /*   int status;
       DMessage *message;

       rt_printf("tconnect : Debut de l'exécution de tconnect\n");

       while (1) {
           rt_printf("tconnect : Attente du sémarphore semConnecterRobot\n");
           rt_sem_p(&semConnecterRobot, TM_INFINITE);
           rt_printf("tconnect : Ouverture de la communication avec le robot\n");
           status = robot->open_device(robot);

           rt_mutex_acquire(&mutexEtat, TM_INFINITE);
           etatCommRobot = status;
           rt_mutex_release(&mutexEtat);

           if (status == STATUS_OK) {
               status = robot->start_insecurely(robot);
               if (status == STATUS_OK){
                   rt_printf("tconnect : Robot démarrer\n");
               }
           }

           message = d_new_message();
           message->put_state(message, status);

           rt_printf("tconnecter : Envoi message\n");
           message->print(message, 100);

           if (write_in_queue(&queueMsgGUI, message, sizeof (DMessage)) < 0) {
               message->free(message);
           }
       }*/
}

void communiquer(void *arg) {
    int datasize = 1;
    char header[4];
    char data[20];
    memset(data, '\0', 20);
    memset(header, '\0', 4);

    printf("tcommuniquer : demarrage\n");
    printf("tserver : Début de l'exécution de serveur\n");
    serverOpen();
    printf("tserver : Connexion\n");

    rt_mutex_acquire(&mutexEtat, TM_INFINITE);
    etatCommMoniteur = 0;
    rt_mutex_release(&mutexEtat);
    
    while(datasize > 0) {
        datasize = receptionFromUI(header, data);
        
        if (strcmp(header, DMB) == 0) {
            printf("EVENEMENT DUMBER DETECTE AVEC LE MESSAGE :%s \n", data);
            /*int a = robotCmd(data[0]);
            printf("Resultat CMD : %d \n", a);
            if (data[0] == 'u' && a == 0) {
                sendToUI(ACK);
            }
            if (data[0] == 'r' && a == 0) {
                sendToUI(ACK);
            }*/
        }

        if (strcmp(header, MES) == 0) {
            printf("EVENEMENT MESSAGE DETECTE AVEC LE MESSAGE :%s \n", data);
        }

        if (strcmp(header, POS) == 0) {
            printf("EVENEMENT POSITION DETECTE AVEC LE MESSAGE :%s \n", data);
        }
    }
    serverClose();
}

void deplacer(void *arg) {
    printf("tdeplacer : demarrage\n");
    /*   int status = 1;
       int gauche;
       int droite;
       DMessage *message;

       rt_printf("tmove : Debut de l'éxecution de periodique à 1s\n");
       rt_task_set_periodic(NULL, TM_NOW, 1000000000);

       while (1) {
           // Attente de l'activation périodique
           rt_task_wait_period(NULL);
           rt_printf("tmove : Activation périodique\n");

           rt_mutex_acquire(&mutexEtat, TM_INFINITE);
           status = etatCommRobot;
           rt_mutex_release(&mutexEtat);

           if (status == STATUS_OK) {
               rt_mutex_acquire(&mutexMove, TM_INFINITE);
               switch (move->get_direction(move)) {
                   case DIRECTION_FORWARD:
                       gauche = MOTEUR_ARRIERE_LENT;
                       droite = MOTEUR_ARRIERE_LENT;
                       break;
                   case DIRECTION_LEFT:
                       gauche = MOTEUR_ARRIERE_LENT;
                       droite = MOTEUR_AVANT_LENT;
                       break;
                   case DIRECTION_RIGHT:
                       gauche = MOTEUR_AVANT_LENT;
                       droite = MOTEUR_ARRIERE_LENT;
                       break;
                   case DIRECTION_STOP:
                       gauche = MOTEUR_STOP;
                       droite = MOTEUR_STOP;
                       break;
                   case DIRECTION_STRAIGHT:
                       gauche = MOTEUR_AVANT_LENT;
                       droite = MOTEUR_AVANT_LENT;
                       break;
               }
               rt_mutex_release(&mutexMove);

               status = robot->set_motors(robot, gauche, droite);

               if (status != STATUS_OK) {
                   rt_mutex_acquire(&mutexEtat, TM_INFINITE);
                   etatCommRobot = status;
                   rt_mutex_release(&mutexEtat);

                   message = d_new_message();
                   message->put_state(message, status);

                   rt_printf("tmove : Envoi message\n");
                   if (write_in_queue(&queueMsgGUI, message, sizeof (DMessage)) < 0) {
                       message->free(message);
                   }
               }
           }
       }*/
}

int write_in_queue(RT_QUEUE *msgQueue, void * data, int size) {
    void *msg;
    (void)msg;
    int err = 0;
    /*
       msg = rt_queue_alloc(msgQueue, size);
        memcpy(msg, &data, size);

        if ((err = rt_queue_send(msgQueue, msg, sizeof (DMessage), Q_NORMAL)) < 0) {
            rt_printf("Error msg queue send: %s\n", strerror(-err));
        }
        rt_queue_free(&queueMsgGUI, msg);
     */
    return err;
}
