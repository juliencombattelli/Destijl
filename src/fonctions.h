/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fonctions.h
 * Author: pehladik
 *
 * Created on 3 mai 2017, 15:12
 */

#ifndef FONCTIONS_H
#define FONCTIONS_H

#include "global.h"
#include "includes.h"

#ifdef	__cplusplus
extern "C" {
#endif
        void connecter (void * arg);
        void communiquer(void *arg);
        void deplacer(void *arg);
        void envoyer(void *arg);
#ifdef	__cplusplus
}
#endif

#endif /* FONCTIONS_H */

