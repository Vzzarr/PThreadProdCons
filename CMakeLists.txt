cmake_minimum_required(VERSION 3.6)
project(HWC1_ProdCons)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/msg_type.h src/msg_type.c src/buffer_type.c src/buffer_type.h
        test/CUnit/Automated.h test/CUnit/Basic.h test/CUnit/Console.h test/CUnit/CUCurses.h test/CUnit/CUError.h
        test/CUnit/CUnit.h test/CUnit/TestDB.h test/CUnit/TestRun.h
        test/test.c)
add_executable(HWC1_ProdCons ${SOURCE_FILES})