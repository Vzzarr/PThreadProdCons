//
// Created by nicholas on 21/11/16.
//
#include "msg_type.h"

#ifndef HWC1_PRODCONS_BUFFER_TYPE_H
#define HWC1_PRODCONS_BUFFER_TYPE_H

typedef struct buffer {
    msg_t* cells;
    int d;  //indice d'inserimento
    int t;  //indice d'estrazione
    int k;  //numero di messaggi presenti
    int size;

    pthread_mutex_t mutex;
    pthread_cond_t notEmpty;
    pthread_cond_t notFull;

    struct buffer* (*buffer_init)(unsigned int);
    void (*buffer_destroy)(struct buffer*);

} buffer_t;

struct arg_struct{
    buffer_t* buffer;
    msg_t* msg;
};

#define BUFFER_ERROR (msg_t *) NULL

/* allocazione / deallocazione buf */
// creazione di un buf vuoto di dim. max nota
buffer_t* buffer_init(unsigned int maxsize);

// deallocazione di un buf
void buffer_destroy(buffer_t* buffer);

/* operazioni sul buf */
// inserimento bloccante: sospende se pieno, quindi
// effettua l’inserimento non appena si libera dello spazio
// restituisce il messaggio inserito; N.B.: msg!=null
msg_t* put_bloccante(buffer_t* buffer, msg_t* msg);

void* do_put_bloccante(void* arguments);

// inserimento non bloccante: restituisce BUFFER_ERROR se pieno,
// altrimenti effettua l’inserimento e restituisce il messaggio
// inserito; N.B.: msg!=null
msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg);

// estrazione bloccante: sospende se vuoto, quindi
// restituisce il valore estratto non appena disponibile
msg_t* get_bloccante(buffer_t* buffer);

// estrazione non bloccante: restituisce BUFFER_ERROR se vuoto
// ed il valore estratto in caso contrario
msg_t* get_non_bloccante(buffer_t* buffer);

//funzioni necessarie per il testing
void* crea_thread(pthread_t pthread1, void* arguments);
void* unisci_thread(pthread_t pthread);



#endif //HWC1_PRODCONS_BUFFER_TYPE_H
