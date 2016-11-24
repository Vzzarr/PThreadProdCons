/*
 *  Simple example of a CUnit unit test.
 *
 *  This program (crudely) demonstrates a very simple "black box"
 *  test of the standard library functions fprintf() and fread().
 *  It uses suite initialization and cleanup functions to open
 *  and close a common temporary file used by the test functions.
 *  The test functions then write to and read from the temporary
 *  file in the course of testing the library functions.
 *
 *  The 2 test functions are added to a single CUnit suite, and
 *  then run using the CUnit Basic interface.  The output of the
 *  program (on CUnit version 2.0-2) is:
 *
 *           CUnit : A Unit testing framework for C.
 *           http://cunit.sourceforge.net/
 *
 *       Suite: Suite_1
 *         Test: test of fprintf() ... passed
 *         Test: test of fread() ... passed
 *
 *       --Run Summary: Type      Total     Ran  Passed  Failed
 *                      suites        1       1     n/a       0
 *                      tests         2       2       2       0
 *                      asserts       5       5       5       0
 */

#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"
#include "../src/buffer_type.h"
#include "../src/buffer_type.c"
#include "../src/msg_type.h"
#include "../src/msg_type.c"

/* Pointer to the file used by the tests. */
static FILE* temp_file = NULL;
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

int clean_suite_bufferSize(void)
{
    buffer_destroy(buffer);
    if (buffer != NULL) {
        return 0;
    }
    else {
        return 1;
    }
}

void test_bufferSize1(void)
{
    if (NULL != buffer) {
        CU_ASSERT(1 == buffer->size);
    }
}

void test_bufferSize2(void)
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











/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /*_________________________BufferSize____________________________________*/
    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_1", init_suite_bufferSize, clean_suite_bufferSize);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
    if ((NULL == CU_add_test(pSuite, "Test Buffer Size 1", test_bufferSize1)) ||
        (NULL == CU_add_test(pSuite, "Test Buffer Size 2", test_bufferSize2)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /*_____________________________________________________________*/
    //•1 (P=1; C=0; N=1) Produzione di un solo messaggio in un buffer vuoto
    pSuite = CU_add_suite("Suite_2", init_suite_P1BE, clean_suite_bufferSize);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
    if ((NULL == CU_add_test(pSuite, "Test P1BE 1", test_P1BE_1)) ||
        (NULL == CU_add_test(pSuite, "Test P1BE 2", test_P1BE_2)))
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
