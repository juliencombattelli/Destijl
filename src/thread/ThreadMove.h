/*
 * ThreadMove.h
 *
 *  Created on: 29 mai 2017
 *      Author: guillaume sarthou
 */

#ifndef SRC_THREAD_THREADMOVE_H_
#define SRC_THREAD_THREADMOVE_H_

/*
 * Ce thread s'occupe de l'envoi apériodique de
 * consignes de mouvement au robot. Pour passer
 * les ordres de mouvement, il suffit de les placer
 * dans la message queue queueMsgRobot.
 * Ce thread modifie également la variable partagée
 * etatCommRobot.
 */
void threadMove(void* arg);



#endif /* SRC_THREAD_THREADMOVE_H_ */
