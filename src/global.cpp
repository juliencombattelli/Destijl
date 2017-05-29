/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "global.h"

RT_TASK tServeur;
RT_TASK tconnect;
RT_TASK tmove;
RT_TASK tenvoyer;
RT_TASK tvideo;
RT_TASK tbattery;
RT_TASK tWatchdog;

RT_MUTEX mutexEtat;
RT_MUTEX mutexMove;

RT_SEM semConnecterRobot;
RT_SEM semConnecterServeur;

RT_QUEUE queueMsgGUI;
RT_QUEUE queueMsgRobot;

int etatCommMoniteur = -1; //initialisation à erreur
int etatCommRobot = -1;    //initialisation à erreur
bool exitApplication = false;

int MSG_QUEUE_SIZE = 20;

int PRIORITY_TSERVEUR = 30;
int PRIORITY_TCONNECT = 35;
int PRIORITY_TMOVE = 45;
int PRIORITY_TENVOYER = 44;
int PRIORITY_TVIDEO = 43;
int PRIORITY_TBATTERY = 34;
int PRIORITY_TWATCHDOG = 99;
