cmake_minimum_required(VERSION 3.1)
project(libdylib)

if(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
endif()

set_source_files_properties(libdylib.h PROPERTIES HEADER_FILE_ONLY TRUE)
add_library(libdylib STATIC libdylib.c libdylib.h)
set_target_properties(libdylib PROPERTIES PREFIX "")
add_library(libdylibxx STATIC libdylibxx.cpp)
set_target_properties(libdylibxx PROPERTIES PREFIX "")

option(BUILD_TESTS BOOL OFF)
if(BUILD_TESTS)
    include_directories(.)
    add_executable(c-tests tests/c-tests.c)
    target_link_libraries(c-tests libdylib ${CMAKE_DL_LIBS})

    add_executable(cpp-tests tests/cpp-tests.cpp)
    target_link_libraries(cpp-tests libdylibxx ${CMAKE_DL_LIBS})

    add_library(testlib SHARED tests/lib.c)
    set_target_properties(testlib PROPERTIES PREFIX "" SUFFIX ".dylib") # For consistency, use "testlib.dylib"
    
    add_library(ptestlib SHARED tests/lib.c)
endif(BUILD_TESTS)
