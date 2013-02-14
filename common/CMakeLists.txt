cmake_minimum_required(VERSION 2.8)
project(common)

add_definitions(-DPROTO_BUF_SUPPORT)
add_definitions(-g)
add_definitions(-DENABLE_LOG_CONSOLE)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)


#####Specify the path of boost
#set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/athrun/boost_1_52_0/stage/include")
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/athrun/boost_trunk/stage/lib")

set(Boost_DIR /home/athrun/boost_1_52_0)
set(CMAKE_MODULE_PATH ${FFNET_ROOT_DIR}/cmake)
include_directories(${PROJECT_SOURCE_DIR} ${Boost_DIR})
link_directories(${Boost_DIR}/stage/lib)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#####Open logs for network
#add_definitions(-DENABLE_LOG_CONSOLE)

set(src_list 	ErrorCode.cpp)
	
add_library(common_dynamic SHARED ${src_list})
add_library(common_static STATIC ${src_list})
target_link_libraries(common_dynamic boost_system boost_date_time boost_thread)
target_link_libraries(common_static boost_system boost_date_time boost_thread)


set_target_properties(common_dynamic PROPERTIES OUTPUT_NAME "common")
set_target_properties(common_static PROPERTIES OUTPUT_NAME "common")
set_target_properties(common_dynamic PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(common_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

