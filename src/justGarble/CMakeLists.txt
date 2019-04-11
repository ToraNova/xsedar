# XSEDAR project
# ToraNova 2019
# Based on justGarble, OTExtension and ENCRYPTO_utils
# ext-src cmake file

cmake_minimum_required(VERSION 3.12)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4 -maes")

find_package(OpenSSL REQUIRED)            # for -lcrypto
find_package (Threads REQUIRED)           # for -lpthread
find_library(SSL_LIB libssl.a REQUIRED)   # for -lssl
find_package( GnuTLS REQUIRED )           # for -gnutlsxx gnutlx

add_library(justgarble
	aes.c
  aescircuits.c
  check.c
  circuits.c
  dkcipher.c
  eval.c
  garble.c
  gates.c
  jgutil.c
  scd.c
  tinyaes.c
  torabuild.c
)

include_directories(${GNUTLS_INCLUDE_DIR})

target_link_libraries(justgarble
  m
  rt
  msgpackc
	OpenSSL::Crypto
  Threads::Threads
  ${SSL_LIB}
  ${GNUTLS_LIBRARIES}
)
