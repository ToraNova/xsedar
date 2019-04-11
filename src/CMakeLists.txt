
cmake_minimum_required(VERSION 3.12)

add_library(xsedar
	xsedar.cpp
)
target_link_libraries(xsedar
	OTExtension::otextension
	ENCRYPTO_utils::encrypto_utils
)

add_executable(setup_test setup_test.cpp)
target_link_libraries(setup_test xsedar)

install(TARGETS setup_test DESTINATION ${PROJECT_SOURCE_DIR}/bin)
