#include "CUnit/Basic.h"
#include "../src/buffer_type.h"

/* Pointer to the file used by the tests. */
static buffer_t* buffer = NULL;

/*_________________________BufferSize____________________________________*/
int init_suite_B1(void)
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
        return -1;
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
void test_P1BE(void){
    msg_t* m = msg_init_string("Mario");

    struct arg_struct ar;
    ar.msg = m;
    ar.buffer = buffer;

    pthread_t p;

    if (NULL != buffer) {
        CU_ASSERT(0 == buffer->k);
        pthread_create(&p, NULL, do_put_bloccante, &ar);
        pthread_join(p, NULL);
        CU_ASSERT(1 == buffer->k);
    }
}

//• (P=0; C=1; N=1) Consumazione di un solo messaggio da un buffer pieno
void test_C1N1(void){
    msg_t* m;

    struct arg_struct ar;
    ar.buffer = buffer;
    pthread_t c;

    if (NULL != buffer) {
        CU_ASSERT(1 == buffer->k);
        pthread_create(&c, NULL, do_get_bloccante, &ar);
        pthread_join(c, &m);
        CU_ASSERT_STRING_EQUAL(m->content, "Mario");
        CU_ASSERT(0 == buffer->k)
    }
}

/*___________________________3__________________________________*/
//• (P=1; C=1; N=1) Consumazione e produzione concorrente di un messaggio da un buffer unitario; prima il consumatore
void test_CPC_C(void){
    msg_t* m = msg_init_string("prova");

    struct arg_struct ar;
    ar.msg = m;
    ar.buffer = buffer;

    pthread_t p;
    pthread_t c;
    if (NULL != buffer) {
        CU_ASSERT(buffer->k == 0);
        pthread_create(&c, NULL, do_get_bloccante, &ar);
        pthread_create(&p, NULL, do_put_bloccante, &ar);
        pthread_join(p, NULL);
        pthread_join(c, NULL);

        CU_ASSERT(buffer->k == 0);
    }
}

//• (P=1; C=1; N=1) Consumazione e produzione concorrente di un messaggio in un buffer unitario; prima il produttore
void test_CPC_P(void){
    msg_t* m = msg_init_string("prova");

    struct arg_struct ar;
    ar.msg = m;
    ar.buffer = buffer;

    pthread_t p;
    pthread_t c;
    if (NULL != buffer) {
        CU_ASSERT(buffer->k == 0);
        pthread_create(&p, NULL, do_put_bloccante, &ar);
        pthread_create(&c, NULL, do_get_bloccante, &ar);

        pthread_join(p, NULL);
        pthread_join(c, NULL);

        CU_ASSERT(buffer->k == 0);
    }
}

/*_____________________________________________________________*/
//più produttori

int init_suite_PP(void)   //più produttori
{
    if (NULL == (buffer = buffer_init(3))) {
        return -1;
    }
    else {
        return 0;
    }
}

//• (P>1; C=0; N>1) Produzione concorrente di molteplici messaggi in un buffer vuoto; il buffer non si riempe
void test_PP1(void)
{
    msg_t* m1 = msg_init_string("MARIO");
    msg_t* m2 = msg_init_string("maria");

    struct arg_struct ar1;
    ar1.msg = m1;
    ar1.buffer = buffer;
    pthread_t pthread1p;
    pthread_t pthread1c;

    struct arg_struct ar2;
    ar2.msg = m2;
    ar2.buffer = buffer;
    pthread_t pthread2p;
    pthread_t pthread2c;

    if (NULL != buffer) {
        pthread_create(&pthread1p, NULL, do_put_bloccante, &ar1);
        pthread_create(&pthread2p, NULL, &do_put_bloccante, &ar2);

        pthread_join(pthread1p, NULL);
        pthread_join(pthread2p, NULL);

        CU_ASSERT(2 == buffer->k);
    }
}

void test_PP2(void)
{
    msg_t* m = msg_init_string("Ma.. Ma..");

    struct arg_struct ar;
    ar.msg = m;
    ar.buffer = buffer;
    pthread_t pthreadp;

    if (NULL != buffer) {
        pthread_create(&pthreadp, NULL, do_put_bloccante, &ar);

        pthread_join(pthreadp, NULL);

        CU_ASSERT(3 == buffer->k);
    }
}




/*_____________________________________________________________*/
//più produttori_più consumatori

int init_suite_PPPC(void)   //più produttori_più consumatori
{
    if (NULL == (buffer = buffer_init(2))) {
        return -1;
    }
    else {
        return 0;
    }
}

void test_PPPC(void)    //non possono essere fatte assunzioni sui messaggi letti
{
    msg_t* m1 = msg_init_string("MARIO");
    msg_t* m2 = msg_init_string("maria");
    msg_t* m3 = msg_init_string("Ma.. Ma..");

    msg_t* msg_get1 = NULL;
    msg_t* msg_get2 = NULL;

    struct arg_struct ar1;
    ar1.msg = m1;
    ar1.buffer = buffer;
    pthread_t pthread1p;
    pthread_t pthread1c;

    struct arg_struct ar2;
    ar2.msg = m2;
    ar2.buffer = buffer;
    pthread_t pthread2p;
    pthread_t pthread2c;

    struct arg_struct ar3;
    ar3.msg = m3;
    ar3.buffer = buffer;
    pthread_t pthread3p;

    if (NULL != buffer) {
        pthread_create(&pthread1p, NULL, do_put_bloccante, (void*)&ar1);
        pthread_create(&pthread2p, NULL, do_put_bloccante, (void*)&ar2);
        pthread_create(&pthread3p, NULL, do_put_bloccante, (void*)&ar3);

        pthread_create(&pthread1c, NULL, do_get_bloccante, (void*)&ar1);
        pthread_create(&pthread2c, NULL, do_get_bloccante, (void*)&ar2);

        pthread_join(pthread1p, NULL);
        pthread_join(pthread2p, NULL);
        pthread_join(pthread3p, NULL);
        pthread_join(pthread1c, (void*)&msg_get1);
        pthread_join(pthread2c, (void*)&msg_get2);

        CU_ASSERT(1 == buffer->k);
    }
}

/*__________________________6___________________________________*/

//• (P>1; C=0; N>1) Produzione concorrente di molteplici messaggi in un buffer vuoto; il buffer si satura in corso
void test_RiempiBuffer(void)   //più produttori_più consumatori
{
    msg_t* m1 = msg_init_string("MARIO");

    struct arg_struct ar1;
    ar1.msg = m1;
    ar1.buffer = buffer;
    pthread_t pthread1p;

    struct arg_struct ar2;
    ar2.msg = m1;
    ar2.buffer = buffer;
    pthread_t pthread2p;

    if (NULL != buffer) {
        CU_ASSERT(buffer->k == 0);

        pthread_create(&pthread1p, NULL, do_put_bloccante, (void*)&ar1);
        pthread_create(&pthread2p, NULL, do_put_bloccante, (void*)&ar2);

        pthread_join(pthread1p, NULL);
        pthread_join(pthread2p, NULL);

        CU_ASSERT(buffer->k == 2);
    }
}

//• (P>1; C=0; N>1) Produzione concorrente di molteplici messaggi in un buffer pieno; il buffer è già saturo
void test_PW(void)   //più produttori_più consumatori
{
    msg_t* m1 = msg_init_string("MARIO");

    struct arg_struct ar1;
    ar1.msg = m1;
    ar1.buffer = buffer;
    pthread_t pthread1p;
    pthread_t pthread1c;


    struct arg_struct ar2;
    ar2.msg = m1;
    ar2.buffer = buffer;
    pthread_t pthread2p;
    pthread_t pthread2c;


    if (NULL != buffer) {

        pthread_create(&pthread1p, NULL, do_put_bloccante, (void*)&ar1);
        pthread_create(&pthread2p, NULL, do_put_bloccante, (void*)&ar2);

        CU_ASSERT(buffer->k == 2);

        pthread_create(&pthread1c, NULL, do_get_bloccante, (void*)&ar1);
        pthread_create(&pthread2c, NULL, do_get_bloccante, (void*)&ar2);

        pthread_join(pthread1p, NULL);
        pthread_join(pthread2p, NULL);
        pthread_join(pthread1c, NULL);
        pthread_join(pthread2c, NULL);

        CU_ASSERT(buffer->k == 2);
    }
}



/*_____________________________________________________________*/
//Non Bloccante

//• (P>1; C=0; N=1) Produzione concorrente di molteplici messaggi in un buffer unitario vuoto
void test_NB_PP(void)   //più produttori, di cui uno tenta di scrivere a buffer pieno
{
    msg_t* m1 = msg_init_string("MARIO");
    msg_t* m2 = msg_init_string("maria");
    msg_t* m3 = msg_init_string("Ma.. Ma..");

    msg_t* msg_get1 = NULL;
    msg_t* msg_get2 = NULL;
    msg_t* msg_get3 = NULL;

    struct arg_struct ar1;
    ar1.msg = m1;
    ar1.buffer = buffer;
    pthread_t pthread1p;

    struct arg_struct ar2;
    ar2.msg = m2;
    ar2.buffer = buffer;
    pthread_t pthread2p;

    struct arg_struct ar3;
    ar3.msg = m3;
    ar3.buffer = buffer;
    pthread_t pthread3p;

    if (NULL != buffer) {
        pthread_create(&pthread1p, NULL, do_put_non_bloccante, (void*)&ar1);
        pthread_create(&pthread2p, NULL, do_put_non_bloccante, (void*)&ar2);
        pthread_create(&pthread3p, NULL, do_put_non_bloccante, (void*)&ar3);

        pthread_join(pthread1p, (void*)&msg_get1);
        pthread_join(pthread2p, (void*)&msg_get2);
        pthread_join(pthread3p, (void*)&msg_get3);

        CU_ASSERT((msg_get1 == BUFFER_ERROR) || (msg_get2 == BUFFER_ERROR) || (msg_get3 == BUFFER_ERROR))
    }
}

//• (P=0; C>1; N=1) Consumazione concorrente di molteplici messaggi da un buffer unitario pieno
void test_NB_PC(void)   //più consumatori, di cui uno tenta di leggere a buffer vuoto
{
    msg_t* msg_get1 = NULL;
    msg_t* msg_get2 = NULL;
    msg_t* msg_get3 = NULL;

    struct arg_struct ar1;
    ar1.buffer = buffer;
    pthread_t pthread1c;

    struct arg_struct ar2;
    ar2.buffer = buffer;
    pthread_t pthread2c;

    struct arg_struct ar3;
    ar3.buffer = buffer;
    pthread_t pthread3c;

    if (NULL != buffer) {
        pthread_create(&pthread1c, NULL, do_get_non_bloccante, (void*)&ar1);
        pthread_create(&pthread2c, NULL, do_get_non_bloccante, (void*)&ar2);
        pthread_create(&pthread3c, NULL, do_get_non_bloccante, (void*)&ar3);

        pthread_join(pthread1c, (void*)&msg_get1);
        pthread_join(pthread2c, (void*)&msg_get2);
        pthread_join(pthread3c, (void*)&msg_get3);

        CU_ASSERT((msg_get1 == BUFFER_ERROR) || (msg_get2 == BUFFER_ERROR) || (msg_get3 == BUFFER_ERROR))
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
    CU_pSuite pSuiteCPC = NULL;
    CU_pSuite pSuitePP = NULL;
    CU_pSuite pSuitePPPC = NULL;
    CU_pSuite pSuitePW = NULL;
    CU_pSuite pSuiteNB = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /*_________________________BufferSize____________________________________*/
    pSuiteSize = CU_add_suite("Suite_1 BL - Base", init_suite_B1, clean_suite);
    if (NULL == pSuiteSize) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuiteSize, "Buffer Size == 1", test_bufferSize))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /*_____________________________________________________________*/
    pSuiteP1BE = CU_add_suite("Suite_2 BL - Minimali", init_suite_B1, clean_suite);
    if (NULL == pSuiteP1BE) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuiteP1BE, "1 Produttore - Buffer unitario vuoto", test_P1BE)) ||
         (NULL == CU_add_test(pSuiteP1BE, "1 Consumatore - Buffer unitario pieno", test_C1N1)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /*_____________________più produttori________________________________________*/
    pSuiteCPC = CU_add_suite("Suite_3 BL - Consumazione/Produzione Concorrente", init_suite_B1, clean_suite);
    if (NULL == pSuiteCPC) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuiteCPC, "1 Consumatore e 1 Produttore concorrenti - Buffer unitario vuoto", test_CPC_C)) ||
        (NULL == CU_add_test(pSuiteCPC, "1 Produttore e 1 Consumatore concorrenti - Buffer unitario vuoto", test_CPC_P)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /*_____________________più produttori________________________________________*/
    pSuitePP = CU_add_suite("Suite_4 BL - Più Produttori", init_suite_PP, clean_suite);
    if (NULL == pSuitePP) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuitePP, "2 Produttori concorrenti - Buffer dim 3 vuoto", test_PP1)) ||
        (NULL == CU_add_test(pSuitePP, "1 Produttore concorrente - Buffer dim 3 con 2 msg", test_PP2)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /*_____________________più produttori_più consumatori________________________________________*/
    pSuitePPPC = CU_add_suite("Suite_5 BL - Più Produttori e Più Consumatori", init_suite_PPPC, clean_suite);
    if (NULL == pSuitePPPC) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuitePPPC, "3 Produttori e 2 Consumatori concorrenti - Buffer dim 2 vuoto", test_PPPC)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /*_____________________PW________________________________________*/
    pSuitePW = CU_add_suite("Suite_6 BL - PW", init_suite_PPPC, clean_suite);
    if (NULL == pSuitePW) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuitePW, "Riempi Buffer", test_RiempiBuffer)) ||
        (NULL == CU_add_test(pSuitePW, "PW", test_PW)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }


    /*_____________________NB-più produttori_più consumatori________________________________________*/
    pSuiteNB = CU_add_suite("Suite_1 NB -", init_suite_B1, clean_suite);
    if (NULL == pSuiteNB) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuiteNB, "Più Produttori concorrenti - Buffer unitario vuoto", test_NB_PP)) ||
        (NULL == CU_add_test(pSuiteNB, "Più Consumatori concorrenti - Buffer unitario vuoto", test_NB_PC)))
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