# XSEDAR project
# ToraNova 2019
# Based on justGarble, OTExtension and ENCRYPTO_utils
# ext-src cmake file

cmake_minimum_required(VERSION 3.12)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4 -maes")

add_library(toralib
	simplesock.c
)

target_link_libraries(toralib
  m
)
