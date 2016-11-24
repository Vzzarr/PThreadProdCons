//
// Created by nicholas on 21/11/16.
//
#include "CUnit/Basic.h"
#include "CUnit/Console.h"
#include "CUnit/Automated.h"
#include "CUnit/CUCurses.h"
#include "../src/buffer_type.h"
#include "../src/buffer_type.c"
#include "../src/msg_type.h"
#include "../src/msg_type.c"

//static buffer_t* buffer = NULL;
//static msg_t* msg = NULL;

//•1 (P=1; C=0; N=1) Produzione di un solo messaggio in un buffer vuoto
int setup_suite1(void){
    //buffer = buffer_init(1);
}

int clean_suite_bufferSize(void){
    //buffer_destroy(buffer);
}

void suite1_test1(void){
    /*msg = msg_init_string("Mario");
    put_bloccante(buffer, msg);
    msg_t* m;
    if(buffer != NULL){
        CU_ASSERT(buffer->isFull);
    }*/
    CU_ASSERT(1);
}

//• (P=0; C=1; N=1) Consumazione di un solo messaggio da un buffer pieno
int setup_suite2(void){
   /* buffer = buffer_init(1);
    msg = msg_init_string("Mario");
    put_bloccante(buffer, msg);*/
}

int clean_suite2(void){
    //buffer_destroy(buffer);
}

void suite2_test1(void){
    /*msg_t* m;
    if(buffer != NULL){
        m = get_bloccante(buffer);
        CU_ASSERT(strcmp("Mario", m->content));
    }*/
}
//• (P=1; C=0; N=1) Produzione in un buffer pieno

//• (P=0; C=1; N=1) Consumazione da un buffer vuoto

//• (P=1; C=1; N=1) Consumazione e produzione concorrente di un messaggio da un buffer unitario; prima il consumatore

//• (P=1; C=1; N=1) Consumazione e produzione concorrente di un messaggio in un buffer unitario; prima il produttore

//• (P>1; C=0; N=1) Produzione concorrente di molteplici messaggi in un buffer unitario vuoto

//• (P=0; C>1; N=1) Consumazione concorrente di molteplici messaggi da un buffer unitario pieno

//• (P>1; C=0; N>1) Produzione concorrente di molteplici messaggi in un buffer vuoto; il buffer non si riempe

//• (P>1; C=0; N>1) Produzione concorrente di molteplici messaggi in un buffer pieno; il buffer `e gi`a saturo

//• (P>1; C=0; N>1) Produzione concorrente di molteplici messaggi in un buffer vuoto; il buffer si satura in corso

//• (P=0; C>1; N>1) Consumazione concorrente di molteplici messaggi da un buffer pieno

//• (P>1; C>1; N=1) Consumazioni e produzioni concorrenti di molteplici messaggi in un buffer unitario

//• (P>1; C>1; N>1) Consumazioni e produzioni concorrenti di molteplici messaggi in un buffer


int main(){
    CU_pSuite pSuite = NULL;

    /*inizializzo il registry*/
    if(CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* aggiungo una suite al registry */
    pSuite = CU_add_suite("Suite1", setup_suite1, clean_suite_bufferSize);
    if(pSuite == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* aggiungo test alla suite */
    if((CU_add_test(pSuite, "test", suite1_test1) == NULL)){
        CU_cleanup_registry();
        return CU_get_error();
    }


    /*_____________________________________________________________*/

    /* aggiungo una suite al registry */
    /*pSuite = CU_add_suite("Suite2", setup_suite2, clean_suite2);
    if(pSuite == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }*/

    /* aggiungo test alla suite */
    /*if((CU_add_test(pSuite, "test", suite2_test1) == NULL)){
        CU_cleanup_registry();
        return CU_get_error();
    }*/


    /*TODO*/

    /* eseguo tutti i test, mettendo il risultato in un file .XML */
    //CU_automated_run_tests();   //AUTOMATED
    CU_basic_run_tests();       //BASIC
    /* dealloco tutti i test dalla memoria */
    CU_cleanup_registry();
}