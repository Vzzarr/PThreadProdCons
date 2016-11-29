//
// Created by nicholas on 21/11/16.
//
#include "buffer_type.h"

buffer_t* buffer_init(unsigned int maxsize){
    buffer_t* new_buffer = (buffer_t*)malloc(sizeof(buffer_t));
    new_buffer->cells = (msg_t*)calloc(maxsize, sizeof(msg_t));
    new_buffer->d = 0;
    new_buffer->t = 0;
    new_buffer->k = 0;
    new_buffer->size = maxsize;

    new_buffer->buffer_init = buffer_init;
    new_buffer->buffer_destroy = buffer_destroy;

    (&new_buffer->mutex, NULL);
    pthread_cond_init(&new_buffer->notEmpty, NULL);
    pthread_cond_init(&new_buffer->notFull, NULL);

    return new_buffer;
}

void buffer_destroy(buffer_t* buffer){
    for (int i = 0; i < buffer->size; ++i)
        buffer->cells[i].msg_destroy;
    free(buffer);
}

msg_t* put_bloccante(buffer_t* buffer, msg_t* msg){
    pthread_mutex_lock(&(buffer->mutex));

    while (buffer->k == buffer->size)
        pthread_cond_wait(&buffer->notFull, &buffer->mutex);

    buffer->cells[buffer->d] = *msg;            //inserisco il messaggio nella cella con indice d
    buffer->d = (buffer->d + 1) % buffer->size; //incremento d in modo circolare
    buffer->k++;                                //incremento il numero di messaggi presenti

    pthread_cond_signal(&buffer->notEmpty);
    pthread_mutex_unlock(&buffer->mutex);

    return msg;
}
void* do_put_bloccante(void* arguments){
    struct arg_struct *ar = arguments;
    msg_t* m = put_bloccante(ar->buffer, ar->msg);
    pthread_exit(&m);
}

// inserimento non bloccante: restituisce BUFFER_ERROR se pieno,
// altrimenti effettua l’inserimento e restituisce il messaggio
// inserito; N.B.: msg!=null
msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg){
    pthread_mutex_lock(&(buffer->mutex));
    if (buffer->k == buffer->size){
        pthread_mutex_unlock(&buffer->mutex);
        return BUFFER_ERROR;
    }
    buffer->cells[buffer->d] = *msg;
    buffer->d = (buffer->d + 1) % buffer->size;
    buffer->k++;

    pthread_mutex_unlock(&buffer->mutex);
    return msg;
}
void* do_put_non_bloccante(void* arguments){
    struct arg_struct *ar = arguments;
    msg_t* m = put_non_bloccante(ar->buffer, ar->msg);
    pthread_exit(m);
}


msg_t* get_bloccante(buffer_t* buffer){
    msg_t* m = NULL;

    pthread_mutex_lock(&buffer->mutex);

    while (buffer->k == 0){
        pthread_cond_wait(&buffer->notEmpty, &buffer->mutex);
    }

    m = &buffer->cells[buffer->t];
    buffer->t = (buffer->t + 1) % buffer->size;
    buffer->k--;

    pthread_cond_signal(&buffer->notFull);
    pthread_mutex_unlock(&buffer->mutex);

    return m;
}
void* do_get_bloccante(void* arguments){
    struct arg_struct *ar = arguments;
    msg_t* m = get_bloccante(ar->buffer);
    pthread_exit(m);
}

// estrazione non bloccante: restituisce BUFFER_ERROR se vuoto
// ed il valore estratto in caso contrario
msg_t* get_non_bloccante(buffer_t* buffer){
    msg_t* m = NULL;

    pthread_mutex_lock(&buffer->mutex);
    if (buffer->k == 0){
        pthread_mutex_unlock(&buffer->mutex);
        return BUFFER_ERROR;
    }
    m = &buffer->cells[buffer->t];
    buffer->t = (buffer->t + 1) % buffer->size;
    buffer->k--;
    pthread_mutex_unlock(&buffer->mutex);

    return m;
}
void* do_get_non_bloccante(void* arguments){
    struct arg_struct *ar = arguments;
    msg_t* m = get_non_bloccante(ar->buffer);
    pthread_exit(m);
}

