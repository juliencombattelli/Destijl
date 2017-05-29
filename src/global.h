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

#define START_MODE WITHOUT_WD

/* @Status */
#define STATUS_OK 0
#define STATUS_ERR -1

/* @descripteurs des tÃ¢ches */
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
extern bool exitApplication;

/* @constantes */
extern int MSG_QUEUE_SIZE;
extern int PRIORITY_TSERVEUR;
extern int PRIORITY_TCONNECT;
extern int PRIORITY_TMOVE;
extern int PRIORITY_TENVOYER;
extern int PRIORITY_TVIDEO;
extern int PRIORITY_TBATTERY;
extern int PRIORITY_TWATCHDOG;


#endif /* GLOBAL_H */

