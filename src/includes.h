/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   includes.h
 * Author: pehladik
 *
 * Created on 3 mai 2017, 15:08
 */

#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <task.h>
#include <mutex.h>
#include <sem.h>
#include <timer.h>
#include <queue.h>

#include <syslog.h>

#include "imagerie.h"
#include "serial.h"
#include "tcpServer.h" // include himself imagerie.h

#endif /* INCLUDES_H */

