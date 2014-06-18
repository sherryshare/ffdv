cmake_minimum_required(VERSION 2.8)
project(manager)

add_definitions(-DPROTO_BUF_SUPPORT)
add_definitions(-g)

#set(FFDV_ROOT_DIR ${PROJECT_SOURCE_DIR}/../../net)
####Specify the path of ffnet/network
set(FFNET_ROOT_DIR /home/sherry/ffnet/network)
#${PROJECT_SOURCE_DIR}/home//../network)

####Specify the path of ffdv/net
set(FFDV_ROOT_DIR /home/sherry/ffdv/net)

#####Specify the path of boost
#set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/athrun/boost_trunk/stage/include")
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/athrun/boost_trunk/stage/lib")

set(Boost_DIR /home/athrun/boost_1_52_0)
set(CMAKE_MODULE_PATH ${FFDV_ROOT_DIR}/cmake)
include_directories(${PROJECT_SOURCE_DIR} ${Boost_DIR} ${FFNET_ROOT_DIR} ${FFDV_ROOT_DIR})
link_directories(${Boost_DIR}/stage/lib ${FFNET_ROOT_DIR}/lib ${FFDV_ROOT_DIR}/lib)


set(svr_src master.cpp)
set(clnt_src client.cpp)

add_executable(master ${svr_src})
target_link_libraries(master network boost_thread protobuf boost_date_time boost_system boost_serialization pthread protobuf net)
add_executable(client_deamon ${clnt_src})
target_link_libraries(client_deamon network boost_thread  protobuf boost_date_time boost_system  boost_serialization pthread protobuf net)
