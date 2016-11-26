//
// Created by nicholas on 21/11/16.
//

#ifndef HWC1_PRODCONS_MSG_TYPE_H
#define HWC1_PRODCONS_MSG_TYPE_H
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct mssg {
    void* content; // generico contenuto del messaggio
    struct msg * (*msg_init)(void*); // creazione msg
    void (*msg_destroy)(struct msg *); // deallocazione msg
    struct msg * (*msg_copy)(struct msg *); // creazione/copia msg
} msg_t;


msg_t* msg_init_string(void* content);

void msg_destroy_string(msg_t* msg);

msg_t* msg_copy_string(msg_t* msg);

//msg t *msg init(void *content);

#endif //HWC1_PRODCONS_MSG_TYPE_H
