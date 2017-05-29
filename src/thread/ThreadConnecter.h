/*
 * ThreadConnecter.h
 *
 *  Created on: 29 mai 2017
 *      Author: guillaume sarthou
 */

#ifndef SRC_THREAD_THREADCONNECTER_H_
#define SRC_THREAD_THREADCONNECTER_H_

/*
 * Ce thread permet la connexion et la déconnexion
 * au robot.
 * Il met ainsi à jour la variable partagée
 * etatCommRobot.
 * Ce thread est synchronisable grace au
 * relachement du sémaphore semConnecterRobot
 */
void threadConnecter(void *arg);



#endif /* SRC_THREAD_THREADCONNECTER_H_ */
