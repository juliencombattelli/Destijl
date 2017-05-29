/*
 * ThreadVideo.h
 *
 *  Created on: 29 mai 2017
 *      Author: guillaume sarthou
 */

#ifndef SRC_THREAD_THREADVIDEO_H_
#define SRC_THREAD_THREADVIDEO_H_

/*
 * ce thread permet l'échantillonnage du flux vidéo,
 * sa compression aisni que son envoie au moniteur.
 * Pour l'envoie de l'image, ce thread passe par
 * la message queue queueMsgGUI.
 * Ce thread ne doit être activé qu'une fois la connexion
 * avec le moniteur effectuée. Pour cela, il peut être
 * synchronisé avec le sémaphore semConnecterServeur.
 * Une fois synchronisé, ce thread devient alors périodique.
 *
 * Il permet également la détection de l'arène et de la position
 * du robot ainsi que l'envoi au moniteur de cette dernière
 * information.
 */
void threadVideo(void *arg);



#endif /* SRC_THREAD_THREADVIDEO_H_ */
