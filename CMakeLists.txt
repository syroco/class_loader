cmake_minimum_required(VERSION 3.1)
project(class_loader CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CLASS_LOADER_ENABLE_TESTING true CACHE BOOL "Build tests")
set(CATKIN_DISABLED false CACHE BOOL "Disable the catkin build, useful if catkin is present but a build outside of ros is done")
if(NOT CATKIN_DISABLED)
  find_package(catkin QUIET)
endif()

add_subdirectory(external/libdylib)

find_package(console_bridge REQUIRED)

if(${catkin_FOUND})
  find_package(catkin REQUIRED COMPONENTS cmake_modules)
  catkin_package(
    INCLUDE_DIRS include
    LIBRARIES ${PROJECT_NAME} ${CMAKE_DL_LIBS}
    DEPENDS console_bridge
    CFG_EXTRAS class_loader-extras.cmake
  )
else()
  set(CATKIN_GLOBAL_BIN_DESTINATION bin)
  set(CATKIN_GLOBAL_LIB_DESTINATION lib)
  set(CATKIN_GLOBAL_LIBEXEC_DESTINATION lib)
  set(CATKIN_GLOBAL_INCLUDE_DESTINATION include)
  set(CATKIN_PACKAGE_LIB_DESTINATION ${CATKIN_GLOBAL_LIB_DESTINATION})
  set(CATKIN_PACKAGE_BIN_DESTINATION ${CATKIN_GLOBAL_LIBEXEC_DESTINATION}/${PROJECT_NAME})
  set(CATKIN_PACKAGE_INCLUDE_DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}/${PROJECT_NAME})
endif()

include_directories(include ${console_bridge_INCLUDE_DIRS} external/)

set(${PROJECT_NAME}_SRCS
  src/class_loader.cpp
  src/class_loader_core.cpp
  src/meta_object.cpp
  src/multi_library_class_loader.cpp
)
set(${PROJECT_NAME}_HDRS
  include/class_loader/class_loader.hpp
  include/class_loader/class_loader_core.hpp
  include/class_loader/exceptions.hpp
  include/class_loader/meta_object.hpp
  include/class_loader/multi_library_class_loader.hpp
  include/class_loader/register_macro.hpp
)
add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS} ${${PROJECT_NAME}_HDRS})
target_link_libraries(${PROJECT_NAME} ${console_bridge_LIBRARIES} libdylibxx ${CMAKE_DL_LIBS})
if(WIN32)
  # Causes the visibility macros to use dllexport rather than dllimport
  # which is appropriate when building the dll but not consuming it.
  target_compile_definitions(${PROJECT_NAME} PRIVATE "CLASS_LOADER_BUILDING_DLL")
endif()

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION})
install(DIRECTORY include/class_loader/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})

install(PROGRAMS scripts/class_loader_headers_update.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

if(CATKIN_ENABLE_TESTING OR CLASS_LOADER_ENABLE_TESTING)
    if(NOT TARGET gtest)
        add_subdirectory(${PROJECT_SOURCE_DIR}/external/googletest-release-1.8.1)
    endif()
    add_subdirectory(test)
endif()

if(NOT catkin_FOUND)
  set(TARGET_NAME ${PROJECT_NAME})
  set(PKGCONFIG_LIBS
    ${console_bridge_LIBRARIES}
    ${CMAKE_DL_LIBS}
  )

  # Prepare and install necessary files to support finding of the library
  # using pkg-config
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/${TARGET_NAME}.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.pc @ONLY)

  install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION lib/pkgconfig)
endif()
