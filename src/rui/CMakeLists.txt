
project( "rui" )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

SET(CMAKE_BUILD_TYPE "Release")
if(MSVC)
	list(APPEND CMAKE_CXX_FLAGS "/std=c++17")
else()
	list(APPEND CMAKE_CXX_FLAGS "-std=c++17")

endif() # if(MSVC)
if (WIN32)
	add_definitions(/Zi)
endif(WIN32)

set_property( GLOBAL PROPERTY USE_FOLDERS ON)
add_compile_options(/wd4819 /wd4100 /wd4244 /wd4189 /wd4200 /wd4815 )

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

file(GLOB_RECURSE WIN_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.inl )
file(GLOB_RECURSE WIN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c)



set( WIN_SRC ${WIN_HEADERS} ${WIN_SOURCES}  )

source_group_by_dir( WIN_SRC )

add_library( "rui" STATIC ${WIN_SRC} )
target_include_directories( "rui" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/src/core" 
                                          "${CMAKE_SOURCE_DIR}/third/glm")

target_link_libraries( "rui" PRIVATE  )
set_target_properties(win PROPERTIES FOLDER "BlowRender/rui" )
