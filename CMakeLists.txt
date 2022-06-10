cmake_minimum_required( VERSION 3.00 )
project ( BlowRender )
set(CMAKE_BUILD_TYPE "Debug" )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set( CMAKE_CXX_STANDARD 17)
set( CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(MSVC)
set_property( GLOBAL PROPERTY USE_FOLDERS ON)
add_compile_options(/wd4819 /wd4100 /wd4244 /wd4189 /wd4200 /wd4815 )
add_definitions(-D _CRT_SECURE_NO_WARNINGS)
set(VULKAN_PATH "C:/VulkanSDK/1.3.211.0")
set(VULKAN_LIB "${VULKAN_PATH}/Lib/Vulkan-1.lib")
else()
set(VULKAN_LIB "/home/chh/Downloads/1.3.211.0/source/Vulkan-Loader/build/loader/libvulkan.so.1")
endif( MSVC)




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


function( CreateStaticLibraryProject  lib_name dir include_dir link_libs depends_lib )
    project( ${lib_name})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    file(GLOB_RECURSE PROJECT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.inl )
    file(GLOB_RECURSE PROJECT_SOURCE  ${CMAKE_CURRENT_SOURCE_DIR}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx ) 
    
    set( PROJECT_SRC ${PROJECT_HEADERS} ${PROJECT_SOURCE} )
    SOURCE_GROUP_BY_DIR( PROJECT_SRC)
    add_library( ${lib_name} STATIC ${PROJECT_SRC})
    if(  ${include_dir} )
        target_include_directories( ${lib_name} PRIVATE ${include_dir} )
    endif( ${include_dir})
    if( ${depends_lib} )
        target_link_libraries( ${lib_name} PRIVATE ${depends_lib})
    endif( ${depends_lib})
    if( ${link_libs})
        target_link_libraries( ${lib_name} PRIAVE ${link_libs})
    endif( ${link_libs})

endfunction()



#1. add request library glad and glfw for to draw window
list( APPEND EXTRA_INCLUDE "${CMAKE_SOURCE_DIR}/third/glad" "${CMAKE_SOURCE_DIR}/third/glm" )
list( APPEND EXTRA_INCLUDE "${CMAKE_SOURCE_DIR}/third")

add_library( glad STATIC "${CMAKE_SOURCE_DIR}/third/glad/glad/glad.c")
if(MSVC)
add_subdirectory( third/glfw)
list( APPEND EXTRA_INCLUDE "${CMAKE_SOURCE_DIR}/third/glfw/include" )
set(glfw_lib glfw)
else()
find_library( glfw_lib NAMES  libglfw.so libglfw.so.3 libglfw3.a PATHS "/usr/local/lib") 
endif(MSVC)
message( "${glfw_lib}")
list( APPEND EXTRA_LIB "${glfw_lib}")

#2. set compiler optionslist( APPEND EXTRA_INCLUDE "${CMAKE_SOURCE_DIR}/third/glfw-3.2.1/include")





add_subdirectory(src)

#3.
option( ENABLE_SAMPLE "Open the sample" ON )
option( ENABLE_UNIT_TEST "Open the unit test" OFF )

if (ENABLE_SAMPLE)
    add_subdirectory( sample )
endif()
if (ENABLE_UNIT_TEST )
	add_subdirectory( unit_test)
endif()
