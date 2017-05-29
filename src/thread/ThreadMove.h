/*
 * ThreadMove.h
 *
 *  Created on: 29 mai 2017
 *      Author: guillaume sarthou
 */

#ifndef SRC_THREAD_THREADMOVE_H_
#define SRC_THREAD_THREADMOVE_H_

/*
 * Ce thread s'occupe de l'envoi ap�riodique de
 * consignes de mouvement au robot. Pour passer
 * les ordres de mouvement, il suffit de les placer
 * dans la message queue queueMsgRobot.
 * Ce thread modifie �galement la variable partag�e
 * etatCommRobot.
 */
void threadMove(void* arg);



#endif /* SRC_THREAD_THREADMOVE_H_ */
