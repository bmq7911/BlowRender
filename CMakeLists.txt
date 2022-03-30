cmake_minimum_required( VERSION 3.00 )
project ( BlowRender )
set(CMAKE_BUILD_TYPE "Debug" )
	
#1. add request library glad and glfw for to draw window



add_library( glad STATIC "${CMAKE_SOURCE_DIR}/third/glad/glad/glad.c")
add_subdirectory( third/glfw-3.2.1  )
find_library( glfw_lib NAMES libglfw3.a glfw3.lib PATHS "${CMAKE_SOURCE_DIR}/build/third/glfw-3.2.1/src"
	                                                    "${CMAKE_SOURCE_DIR}/build/third/glfw-3.2.1/src/Debug"
							                            "${CMAKE_SOURCE_DIR}/build/thrid/glfw-3.2.1/src/Release") 

set( TEST_VARIABLE "hello cmake")
list( APPEND EXTRA_LIB glad "${glfw_lib}")


#2. set compiler options
set( CMAKE_CXX_STANDARD 11)
set( CMAKE_CXX_STANDARD_REQUIRED TRUE)
add_compile_options(/wd4819)
add_definitions(-D _CRT_SECURE_NO_WARNINGS)

list( APPEND EXTRA_INCLUDE "${CMAKE_SOURCE_DIR}/third/glfw-3.2.1/include")
list( APPEND EXTRA_INCLUDE "${CMAKE_SOURCE_DIR}/third/glad" "${CMAKE_SOURCE_DIR}/third/glm" )
list( APPEND EXTRA_INCLUDE "${CMAKE_SOURCE_DIR}/third")


add_subdirectory(src)

option( ENABLE_SAMPLE "Open the sample" ON )
option( ENABLE_UNIT_TEST "Open the unit test" OFF )

if (ENABLE_SAMPLE)
    add_subdirectory( sample )
endif()
if (ENABLE_UNIT_TEST )
	add_subdirectory( unit_test)
endif()
