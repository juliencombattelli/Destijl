/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   global.h
 * Author: pehladik
 *
 * Created on 3 mai 2017, 15:09
 */

#ifndef GLOBAL_H
#define GLOBAL_H


#include "includes.h"

/* @descripteurs des tâches */
extern RT_TASK tServeur;
extern RT_TASK tconnect;
extern RT_TASK tmove;
extern RT_TASK tenvoyer;
extern RT_TASK tvideo;
extern RT_TASK tbattery;
extern RT_TASK tWatchdog;

/* @descripteurs des mutex */
extern RT_MUTEX mutexEtat;
extern RT_MUTEX mutexMove;

/* @descripteurs des sempahore */
extern RT_SEM semConnecterRobot;
extern RT_SEM semConnecterServeur;

/* @descripteurs des files de messages */
extern RT_QUEUE queueMsgGUI;
extern RT_QUEUE queueMsgRobot;

/* @variables partagées */
extern int etatCommMoniteur;
extern int etatCommRobot;
/*extern DServer *serveur;
extern DRobot *robot;
extern DMovement *move;*/

/* @constantes */
extern int MSG_QUEUE_SIZE;
extern int PRIORITY_TSERVEUR;
extern int PRIORITY_TCONNECT;
extern int PRIORITY_TMOVE;
extern int PRIORITY_TENVOYER;

#endif /* GLOBAL_H */

