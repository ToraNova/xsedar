# XSEDAR project
# ToraNova 2019
# Based on justGarble, OTExtension and ENCRYPTO_utils
# src cmake file

cmake_minimum_required(VERSION 3.12)

add_subdirectory(justGarble)
include_directories(justGarble)

add_library(xsedar
	xsedar.cpp
)

target_link_libraries(xsedar
	OTExtension::otextension
	ENCRYPTO_utils::encrypto_utils
)

add_executable(setup_test setup_test.cpp)
target_link_libraries(setup_test 
	xsedar justgarble ENCRYPTO_utils::encrypto_utils)

install(TARGETS setup_test DESTINATION ${PROJECT_SOURCE_DIR}/bin)
