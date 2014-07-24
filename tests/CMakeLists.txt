cmake_minimum_required(VERSION 2.8.8)
 
set(PROJECT_NAME nokkhum-processor)
set(PROJECT_TEST_NAME ${PROJECT_NAME}-test)
project(${PROJECT_NAME} C CXX)
 
 
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/external)
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
 
# define library usage
find_package(PkgConfig)
pkg_check_modules(OPENCV REQUIRED "opencv >= 2.4")
pkg_check_modules(LIBGLOG REQUIRED "libglog >= 0.3")
pkg_check_modules(LIBJSONCPP REQUIRED "jsoncpp")

set (BOOST_LIBRARIES "-lboost_filesystem -lboost_system -lboost_date_time -lboost_program_options")
set (OPENCV_LIBRARIES "-lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -lopencv_video -lopencv_features2d -lopencv_contrib")
set (JSONCPP_LIBRARIES "-ljsoncpp")
set (POKO_LIBRARIES "-lPocoFoundation")
#set (EMAMI_LIBRARIES "-lemami_facereg")
set (EXTRA_LIBS "${PROJECT_SOURCE_DIR}/../emami_facereg/libemami_facereg.so")


link_libraries(
   ${OPENCV_LIBRARIES}
   ${BOOST_LIBRARIES}
   ${LIBGLOG_LIBRARIES}
   ${JSONCPP_LIBRARIES}
   ${POKO_LIBRARIES}
   #${EMAMI_LIBRARIES}
)
   
 
file(GLOB_RECURSE ${PROJECT_NAME}_CXX_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../src/*.cpp")
file(GLOB_RECURSE ${PROJECT_NAME}_CXX_TESTS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "test*.cpp")
 
# remove main function file from test
list(REMOVE_ITEM ${PROJECT_NAME}_CXX_SRC ${CMAKE_CURRENT_SOURCE_DIR} "../src/main.cpp")
 
# complier
set (CMAKE_CXX_COMPILER g++)
add_definitions(-std=c++1y -pthread)
set (CMAKE_VERBOSE_MAKEFILE TRUE)
 
set (CMAKE_CXX_FLAGS_DEBUG, "-g -Wall")
set (CMAKE_CXX_FLAGS "-Wall")
 
include_directories ("${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/../src"  "${PROJECT_SOURCE_DIR}/../tests" "${PROJECT_SOURCE_DIR}/../emami_facereg/include")
 
message(STATUS ${EXTRA_LIBS} )
add_executable (${PROJECT_TEST_NAME} ${${PROJECT_NAME}_CXX_TESTS} ${${PROJECT_NAME}_CXX_SRC})
# add_executable (${PROJECT_TEST_NAME} ${${PROJECT_NAME}_CXX_SRC})
add_dependencies(${PROJECT_TEST_NAME} googletest)
target_link_libraries (${PROJECT_TEST_NAME} 
    ${GTEST_LIBS_DIR}/libgtest.a
    ${GTEST_LIBS_DIR}/libgtest_main.a
    ${EXTRA_LIBS}
    pthread
)
