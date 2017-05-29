/*
 * ThreadEnvoyer.h
 *
 *  Created on: 29 mai 2017
 *      Author: guillaume sarthou
 */

#ifndef SRC_THREAD_THREADENVOYER_H_
#define SRC_THREAD_THREADENVOYER_H_

/*
 * Ce thread gère l'envoie de message au moniteur.
 * Ainsi, tout message à destination de ce dernier
 * se doit de passer par se thread.
 * Pour cela le message doit être placé das la message
 * queue queueMsgGUI.
 * Ce thread s'occupe aussi de la construction de
 * message plus complexe au moniteur en permettant
 * l'envoi d'argument. Pour cela, l'identificateur
 * du message (ex : BAT) doit être passé avant les arguments
 * dans la file tout deux avec la même gestion des priorités.
 */
void threadEnvoyer(void *arg);



#endif /* SRC_THREAD_THREADENVOYER_H_ */
