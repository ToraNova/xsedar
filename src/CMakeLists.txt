
cmake_minimum_required(VERSION 3.12)

add_executable(xsedar xsedar.cpp)
target_link_libraries(xsedar 
	OTExtension::otextension
	ENCRYPTO_utils::encrypto_utils
	)

