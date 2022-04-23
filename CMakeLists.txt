cmake_minimum_required( VERSION 3.00 )
project ( BlowRender )
set(CMAKE_BUILD_TYPE "Debug" )
	
if(MSVC)
set_property( GLOBAL PROPETY USE_FOLDERS ON)
add_compile_options(/wd4819)
add_definitions(-D _CRT_SECURE_NO_WARNINGS)
endif(MSVC)

macro(source_group_by_dir source_files)
    if(MSVC)
        set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        foreach(sgbd_file ${${source_files}})
            string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
			if( ${sgbd_file} MATCHES ".+(.h|.hpp|.inl)$"  )
				source_group(${sgbd_group_name}\\public FILES ${sgbd_file})
			elseif( ${sgbd_file} MATCHES ".+(.c|.cpp)$" )
				source_group(${sgbd_group_name}\\private FILES ${sgbd_file})
			else()
				source_group(${sgbd_group_name}\\resource FILES ${sgbd_file})
			endif(${sgbd_file} MATCHES ".+(.h|.hpp|.inl)$" )
			
        endforeach(sgbd_file)
    endif(MSVC)
endmacro(source_group_by_dir)


#1. add request library glad and glfw for to draw window

add_library( glad STATIC "${CMAKE_SOURCE_DIR}/third/glad/glad/glad.c")
add_subdirectory( third/glfw-3.2.1  )
find_library( glfw_lib NAMES libglfw3.a glfw3.lib PATHS "${CMAKE_SOURCE_DIR}/build/third/glfw-3.2.1/src"
	                                                    "${CMAKE_SOURCE_DIR}/build/third/glfw-3.2.1/src/Debug"
							                            "${CMAKE_SOURCE_DIR}/build/thrid/glfw-3.2.1/src/Release") 

set( TEST_VARIABLE "hello cmake")
list( APPEND EXTRA_LIB glad "${glfw_lib}")


#2. set compiler options
set( CMAKE_CXX_STANDARD 17)
set( CMAKE_CXX_STANDARD_REQUIRED TRUE)

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
