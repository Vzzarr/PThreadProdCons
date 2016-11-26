#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "CUnit/Basic.h"
#include "../src/buffer_type.h"
#include "../src/buffer_type.c"
#include "../src/msg_type.h"
#include "../src/msg_type.c"

/* Pointer to the file used by the tests. */
static buffer_t* buffer = NULL;

/*_________________________BufferSize____________________________________*/
int init_suite_bufferSize(void)
{
    if (NULL == (buffer = buffer_init(1))) {
        return -1;
    }
    else {
        return 0;
    }
}

int clean_suite(void)
{
    buffer_destroy(buffer);
    if (buffer != NULL) {
        return 0;
    }
    else {
        return 1;
    }
}

void test_bufferSize(void)
{
    if (NULL != buffer) {
        CU_ASSERT(1 == buffer->size);
    }
}


/*_____________________________________________________________*/
//•1 (P=1; C=0; N=1) Produzione di un solo messaggio in un buffer vuoto
int init_suite_P1BE(void)   //production 1  Buffer Empty
{
    if (NULL == (buffer = buffer_init(1))) {
        return -1;
    }
    else {
        return 0;
    }
}

void test_P1BE_1(void)
{
    if (NULL != buffer) {
        CU_ASSERT(0 == buffer->k);
    }
}

void test_P1BE_2(void)
{
    msg_t* m = msg_init_string("Mario");
    if (NULL != buffer) {
        put_bloccante(buffer, m);
        CU_ASSERT(1 == buffer->k);
    }
}

/*_____________________________________________________________*/
//più produttori_più consumatori

int init_suite_PPPC(void)   //più produttori_più consumatori
{
    if (NULL == (buffer = buffer_init(3))) {
        return -1;
    }
    else {
        return 0;
    }
}

void test_PPPC(void)
{
    msg_t* m1 = msg_init_string("MARIO");
    msg_t* m2 = msg_init_string("maria");
    msg_t* m3 = msg_init_string("Ma.. Ma..");

    struct arg_struct *ar1;
    ar1->msg = m1;
    ar1->buffer = buffer;
    pthread_t pthread1;

    ar2->msg = m2;
    ar2->buffer = buffer;
    pthread_t pthread2;

    struct arg_struct *ar3;
    ar3->msg = m3;
    ar3->buffer = buffer;
    pthread_t pthread3;

    if (NULL != buffer) {
        pthread_create(&pthread1, NULL, do_put_bloccante, ar1);
        pthread_create(&pthread2, NULL, do_put_bloccante, ar2);
        pthread_create(&pthread3, NULL, do_put_bloccante, ar3);
        //crea_thread(pthread1, ar1);
        //crea_thread(pthread2, ar2);
        //crea_thread(pthread3, ar3);

        pthread_join(pthread1, NULL);
        pthread_join(pthread2, NULL);
        pthread_join(pthread3, NULL);
        //unisci_thread(pthread1);
        //unisci_thread(pthread2);
        //unisci_thread(pthread3);

        CU_ASSERT(3 == buffer->k);
    }
}





/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
    CU_pSuite pSuiteSize = NULL;
    CU_pSuite pSuiteP1BE = NULL;
    CU_pSuite pSuitePPPC = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /*_________________________BufferSize____________________________________*/
    pSuiteSize = CU_add_suite("Suite_1", init_suite_bufferSize, clean_suite);
    if (NULL == pSuiteSize) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuiteSize, "Test Buffer Size 1", test_bufferSize))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /*_____________________________________________________________*/
    //•1 (P=1; C=0; N=1) Produzione di un solo messaggio in un buffer vuoto
    pSuiteP1BE = CU_add_suite("Suite_2", init_suite_P1BE, clean_suite);
    if (NULL == pSuiteP1BE) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuiteP1BE, "Test P1BE 1", test_P1BE_1)) ||
        (NULL == CU_add_test(pSuiteP1BE, "Test P1BE 2", test_P1BE_2)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /*_____________________più produttori_più consumatori________________________________________*/
    pSuitePPPC = CU_add_suite("Suite_3", init_suite_PPPC, clean_suite);
    if (NULL == pSuitePPPC) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuitePPPC, "Test P1BE 1", test_PPPC)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }


    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
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