
include_directories(${PROJECT_SOURCE_DIR}/include)
include(${PROJECT_SOURCE_DIR}/cmake/class_loader-extras.cmake)

add_library(${PROJECT_NAME}_TestPlugins1 EXCLUDE_FROM_ALL SHARED plugins1.cpp)
target_link_libraries(${PROJECT_NAME}_TestPlugins1 ${PROJECT_NAME})
class_loader_hide_library_symbols(${PROJECT_NAME}_TestPlugins1)
add_library(${PROJECT_NAME}_TestPlugins2 EXCLUDE_FROM_ALL SHARED plugins2.cpp)
target_link_libraries(${PROJECT_NAME}_TestPlugins2 ${PROJECT_NAME})
class_loader_hide_library_symbols(${PROJECT_NAME}_TestPlugins2)

if(${catkin_FOUND})
    catkin_add_gtest(${PROJECT_NAME}_utest utest.cpp)
    catkin_add_gtest(${PROJECT_NAME}_shared_ptr_test shared_ptr_test.cpp)
    catkin_add_gtest(${PROJECT_NAME}_unique_ptr_test unique_ptr_test.cpp)
else()
    enable_testing()
    add_executable(${PROJECT_NAME}_utest utest.cpp)
    add_executable(${PROJECT_NAME}_shared_ptr_test shared_ptr_test.cpp)
    add_executable(${PROJECT_NAME}_unique_ptr_test unique_ptr_test.cpp)
    add_test(NAME ${PROJECT_NAME}_utest COMMAND ${PROJECT_NAME}_utest )
    add_test(NAME ${PROJECT_NAME}_shared_ptr_test COMMAND ${PROJECT_NAME}_shared_ptr_test )
    add_test(NAME ${PROJECT_NAME}_unique_ptr_test COMMAND ${PROJECT_NAME}_unique_ptr_test )
endif()

target_link_libraries(${PROJECT_NAME}_utest ${PROJECT_NAME} gtest gtest_main)
add_dependencies(${PROJECT_NAME}_utest ${PROJECT_NAME}_TestPlugins1 ${PROJECT_NAME}_TestPlugins2)

target_link_libraries(${PROJECT_NAME}_shared_ptr_test ${PROJECT_NAME} gtest gtest_main)
add_dependencies(${PROJECT_NAME}_shared_ptr_test ${PROJECT_NAME}_TestPlugins1 ${PROJECT_NAME}_TestPlugins2)


target_link_libraries(${PROJECT_NAME}_unique_ptr_test ${PROJECT_NAME} gtest gtest_main)
add_dependencies(${PROJECT_NAME}_unique_ptr_test ${PROJECT_NAME}_TestPlugins1 ${PROJECT_NAME}_TestPlugins2)
