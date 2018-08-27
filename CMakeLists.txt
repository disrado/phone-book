cmake_minimum_required(VERSION 3.10)

project(phone-book-project LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup()

if (ENABLE_COVERAGE)
	include(Coverage)
endif ()

include(cotire)

add_subdirectory(modules/libpb)
add_subdirectory(modules/app)

if (ENABLE_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif ()
