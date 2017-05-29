/*
 * ThreadVideo.h
 *
 *  Created on: 29 mai 2017
 *      Author: guillaume sarthou
 */

#ifndef SRC_THREAD_THREADVIDEO_H_
#define SRC_THREAD_THREADVIDEO_H_

/*
 * ce thread permet l'�chantillonnage du flux vid�o,
 * sa compression aisni que son envoie au moniteur.
 * Pour l'envoie de l'image, ce thread passe par
 * la message queue queueMsgGUI.
 * Ce thread ne doit �tre activ� qu'une fois la connexion
 * avec le moniteur effectu�e. Pour cela, il peut �tre
 * synchronis� avec le s�maphore semConnecterServeur.
 * Une fois synchronis�, ce thread devient alors p�riodique.
 *
 * Il permet �galement la d�tection de l'ar�ne et de la position
 * du robot ainsi que l'envoi au moniteur de cette derni�re
 * information.
 */
void threadVideo(void *arg);



#endif /* SRC_THREAD_THREADVIDEO_H_ */
