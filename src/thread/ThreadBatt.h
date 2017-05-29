/*
 * ThreadBatt.h
 *
 *  Created on: 29 mai 2017
 *      Author: guillaume sarthou
 */

#ifndef SRC_THREAD_THREADBATT_H_
#define SRC_THREAD_THREADBATT_H_


/*
 * G�re le relev� p�riodique du niveau de la batterie
 * et l'envoie � travers la message queue queueMsgGUI
 * au moniteur.
 * Ce thread met �galement � jour l'�tat de la communication
 * avec le robot
 */
void threadBatt(void* arg);



#endif /* SRC_THREAD_THREADBATT_H_ */
