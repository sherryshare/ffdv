cmake_minimum_required(VERSION 2.8)
project(os)

add_definitions(-DPROTO_BUF_SUPPORT)
add_definitions(-g)
add_definitions(-DENABLE_LOG_CONSOLE)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)



#####Specify the path of boost
#set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/athrun/boost_1_52_0/stage/include")
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/athrun/boost_trunk/stage/lib")

####Specify the path of ffnet/network
set(FFNET_ROOT_DIR /home/sherry/ffnet-master/network)
set(Boost_DIR /home/athrun/boost_1_52_0)
#set(CMAKE_MODULE_PATH ${FFDV_ROOT_DIR}/cmake)


include_directories(${PROJECT_SOURCE_DIR} ${Boost_DIR} ${FFNET_ROOT_DIR})
link_directories(${Boost_DIR}/stage/lib ${FFNET_ROOT_DIR}/lib)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#####Open logs for network
#add_definitions(-DENABLE_LOG_CONSOLE)

set(src 	SysState.cpp)
	
add_library(os_dynamic SHARED ${src})
add_library(os_static STATIC ${src})
target_link_libraries(os_dynamic network boost_thread protobuf boost_date_time boost_system)
target_link_libraries(os_static network boost_thread protobuf boost_date_time boost_system)


set_target_properties(os_dynamic PROPERTIES OUTPUT_NAME "os")
set_target_properties(os_static PROPERTIES OUTPUT_NAME "os")
set_target_properties(os_dynamic PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(os_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
