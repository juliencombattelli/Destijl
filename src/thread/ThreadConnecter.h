/*
 * ThreadConnecter.h
 *
 *  Created on: 29 mai 2017
 *      Author: guill
 */

#ifndef SRC_THREAD_THREADCONNECTER_H_
#define SRC_THREAD_THREADCONNECTER_H_

/*
 * Ce thread permet la connexion et la d�connexion
 * au robot.
 * Il met ainsi � jour la variable partag�e
 * etatCommRobot.
 * Ce thread est synchronisable grace au
 * relachement du s�maphore semConnecterRobot
 */
void threadConnecter(void *arg);



#endif /* SRC_THREAD_THREADCONNECTER_H_ */
