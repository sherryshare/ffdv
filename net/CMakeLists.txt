cmake_minimum_required(VERSION 2.8)
project(net)

add_definitions(-DPROTO_BUF_SUPPORT)
add_definitions(-g)
add_definitions(-DENABLE_LOG_CONSOLE)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)


####Specify the path of ffnet/network
set(FFNET_ROOT_DIR /home/sherry/ffnet-master/network)
#${PROJECT_SOURCE_DIR}/home//../network)

#####Specify the path of boost
#set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/athrun/boost_trunk/stage/include")
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/athrun/boost_trunk/stage/lib")

set(Boost_DIR /home/athrun/boost_1_52_0)
set(CMAKE_MODULE_PATH ${FFDV_ROOT_DIR}/cmake)
include_directories(${PROJECT_SOURCE_DIR} ${Boost_DIR} ${FFNET_ROOT_DIR})
link_directories(${Boost_DIR}/stage/lib ${FFNET_ROOT_DIR}/lib)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#####Open logs for network
#add_definitions(-DENABLE_LOG_CONSOLE)

set(src client_event_handler.cpp 
	master_event_handler.cpp
	net_layer.cpp 
	message.pb.cc
)

add_library(net_dynamic SHARED ${src})
add_library(net_static STATIC ${src})
#add_library(net SHARED ${src})
#add_executable(master ${src})
#target_link_libraries(net network boost_thread  protobuf boost_date_time boost_system  boost_serialization pthread protobuf)

target_link_libraries(net_dynamic network boost_thread  protobuf boost_date_time boost_system  boost_serialization pthread protobuf)
target_link_libraries(net_static network boost_thread  protobuf boost_date_time boost_system  boost_serialization pthread protobuf)


set_target_properties(net_dynamic PROPERTIES OUTPUT_NAME "net")
set_target_properties(net_static PROPERTIES OUTPUT_NAME "net")
set_target_properties(net_dynamic PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(net_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)