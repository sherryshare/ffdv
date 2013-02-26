cmake_minimum_required(VERSION 2.8)
project(test)

add_definitions(-DPROTO_BUF_SUPPORT)
add_definitions(-g)
add_definitions(-DENABLE_LOG_CONSOLE)

####Specify the path of ffnet/network
set(FFNET_ROOT_DIR /home/sherry/ffnet-master/network)
set(Boost_DIR /home/athrun/boost_1_52_0)
####Specify the path of ffdv/os
set(OS_ROOT_DIR /home/sherry/ffdv/os)

#####Specify the path of boost
#set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/athrun/boost_trunk/stage/include")
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/athrun/boost_trunk/stage/lib")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include_directories(${PROJECT_SOURCE_DIR} ${OS_ROOT_DIR} ${Boost_DIR} ${FFNET_ROOT_DIR})
link_directories(${OS_ROOT_DIR}/lib ${Boost_DIR}/stage/lib ${FFNET_ROOT_DIR}/lib)

#add_definitions(-DENABLE_LOG_CONSOLE

set(test_src main.cpp)

add_executable(test_main ${test_src})
target_link_libraries(test_main os network boost_thread protobuf boost_date_time boost_system)