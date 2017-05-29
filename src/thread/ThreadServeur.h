/*
 * ThreadServeur.h
 *
 *  Created on: 29 mai 2017
 *      Author: guillaume sarthou
 */

#ifndef SRC_THREAD_THREADSERVEUR_H_
#define SRC_THREAD_THREADSERVEUR_H_

/*
 * Ce thread permet de g�rer l'ouverture de la communication
 * TCP ainsi que sa connexion.
 * Il permet aussi d'interpr�ter les messages �mis par le
 * moniteur et ainsi d'appliquer l'action correspondant en
 * donnant la main  au diff�rents autres thread.
 */
void threadServeur(void *arg);

#endif /* SRC_THREAD_THREADSERVEUR_H_ */
