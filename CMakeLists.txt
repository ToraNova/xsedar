
cmake_minimum_required(VERSION 3.12)

find_package(Threads REQUIRED)
find_package(Boost 1.69.0 COMPONENTS system thread filesystem REQUIRED)
find_package(ENCRYPTO_utils REQUIRED)
find_package(OTExtension REQUIRED)
if(OTExtension_FOUND)
	message("Found package OTExtension")
endif()
if(ENCRYPTO_utils_FOUND)
	message("Found package ENCRYPTO_utils")
endif()

add_subdirectory(src)


