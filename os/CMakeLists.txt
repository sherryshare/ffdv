cmake_minimum_required(VERSION 2.8)
project(os)

add_definitions(-DPROTO_BUF_SUPPORT)
add_definitions(-g)
add_definitions(-DENABLE_LOG_CONSOLE)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#####Specify the path of common
set(COMMON_ROOT_DIR /home/sherry/ffdv/common)

#####Specify the path of boost
#set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/athrun/boost_1_52_0/stage/include")
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/athrun/boost_trunk/stage/lib")


include_directories(${PROJECT_SOURCE_DIR} ${COMMON_ROOT_DIR})
link_directories(${COMMON_ROOT_DIR}/lib)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#####Open logs for network
#add_definitions(-DENABLE_LOG_CONSOLE)

set(src_list 	sys_detail/SysLinux2_6_18.cpp
	sys_detail/SysLinux2_6_26.cpp
	sys_detail/SysLinux2_6_32.cpp
	sys_detail/SysLinux2_6_33.cpp
	sys_detail/SysLinux3_2_0.cpp
	SysState.cpp
)
	
add_library(os_dynamic SHARED ${src_list})
add_library(os_static STATIC ${src_list})
target_link_libraries(os_dynamic common)
target_link_libraries(os_static common)


set_target_properties(os_dynamic PROPERTIES OUTPUT_NAME "os")
set_target_properties(os_static PROPERTIES OUTPUT_NAME "os")
set_target_properties(os_dynamic PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(os_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

