#include <client_registry.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "csapp.h"
#include "debug.h"
#include "crecorycal.h"

CLIENT_REGISTRY *head;

/*
 * Initialize a new thread counter.
 */
CLIENT_REGISTRY *creg_init(){
    CLIENT_REGISTRY* cr = Malloc(sizeof(CLIENT_REGISTRY));
    void *pntr = cr;
    head = cr;
    (*(CLIENT_REGISTRY *)pntr).num=0;
    (*(CLIENT_REGISTRY *)pntr).next= NULL;
    Sem_init(&(*(CLIENT_REGISTRY *)pntr).sem, 0, 0);
    pthread_mutex_init(&((*(CLIENT_REGISTRY *)pntr).mutex), NULL);
   // pthread_mutex_init(&(head->mutex), NULL);
    return cr;
}

void creg_fini(CLIENT_REGISTRY *cr){
    void *pntr = cr;
    CLIENT_REGISTRY *dupCr;
    do{
        dupCr= (*(CLIENT_REGISTRY *)pntr).next;
        Free(cr);
        cr= dupCr;
    }while(dupCr!=NULL);
}

void creg_register(CLIENT_REGISTRY *cr, int fd){
    pthread_mutex_lock(&(head->mutex));
    CLIENT_REGISTRY *clientRef= cr;
    void *pntr = clientRef;
    while((*(CLIENT_REGISTRY *)pntr).next!=NULL){
      clientRef = (*(CLIENT_REGISTRY *)pntr).next;
    }

    CLIENT_REGISTRY* newCr= Malloc(sizeof(CLIENT_REGISTRY));
    void *new_pntr = newCr;
    (*(CLIENT_REGISTRY *)new_pntr).fd= fd;
    (*(CLIENT_REGISTRY *)new_pntr).next = newCr;
    head->num++;
    (*(CLIENT_REGISTRY *)new_pntr).next= NULL;
    pthread_mutex_unlock(&(head->mutex));
}

void creg_unregister(CLIENT_REGISTRY *cr, int fd){
    void *pntr = cr;
    pthread_mutex_lock(&(head->mutex));
    if((*(CLIENT_REGISTRY *)pntr).next==NULL){
        pthread_mutex_unlock(&(head->mutex));
        return;
    }
    CLIENT_REGISTRY *clientRef= cr;
    pntr = clientRef;
    CLIENT_REGISTRY *nextClientRef;
    if(clientRef!=NULL){
         while((*(CLIENT_REGISTRY *)pntr).next->fd != fd){
             clientRef = (*(CLIENT_REGISTRY *)pntr).next;
             if(clientRef==NULL){
                pthread_mutex_unlock(&(head->mutex));
                return;
             }
         }
       nextClientRef= (*(CLIENT_REGISTRY *)pntr).next->next;
    }
    Free((*(CLIENT_REGISTRY *)pntr).next);
    (*(CLIENT_REGISTRY *)pntr).next= nextClientRef;
    head->num--;
    pthread_mutex_unlock(&(head->mutex));
}

void creg_wait_for_empty(CLIENT_REGISTRY *cr){
     while(1){
        if(head->num==0){
            break;
        }
     }

}

void creg_shutdown_all(CLIENT_REGISTRY *client){

    pthread_mutex_lock(&(head->mutex));
    CLIENT_REGISTRY *cr= client;
    void *pntr = cr;
    while((*(CLIENT_REGISTRY *)pntr).next != NULL){
        head->num--;
        cr = (*(CLIENT_REGISTRY *)pntr).next;
        shutdown((*(CLIENT_REGISTRY *)pntr).fd, SHUT_RD);
    }
    pthread_mutex_unlock(&(head->mutex));

}