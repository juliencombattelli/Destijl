/*
 * ThreadEnvoyer.h
 *
 *  Created on: 29 mai 2017
 *      Author: guillaume sarthou
 */

#ifndef SRC_THREAD_THREADENVOYER_H_
#define SRC_THREAD_THREADENVOYER_H_

/*
 * Ce thread g�re l'envoie de message au moniteur.
 * Ainsi, tout message � destination de ce dernier
 * se doit de passer par se thread.
 * Pour cela le message doit �tre plac� das la message
 * queue queueMsgGUI.
 * Ce thread s'occupe aussi de la construction de
 * message plus complexe au moniteur en permettant
 * l'envoi d'argument. Pour cela, l'identificateur
 * du message (ex : BAT) doit �tre pass� avant les arguments
 * dans la file tout deux avec la m�me gestion des priorit�s.
 */
void threadEnvoyer(void *arg);



#endif /* SRC_THREAD_THREADENVOYER_H_ */
