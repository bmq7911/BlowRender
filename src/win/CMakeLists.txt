
project( "win" )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(GLOB_RECURSE WIN_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.inl )
file(GLOB_RECURSE WIN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c)


set( WIN_SRC ${WIN_HEADERS} ${WIN_SOURCES}  )

source_group_by_dir( WIN_SRC )

add_library( "win" STATIC ${WIN_SRC} )
target_include_directories( "win" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/src/core" "${CMAKE_SOURCE_DIR}/third/glad" 
                                          "${CMAKE_SOURCE_DIR}/third/glm")

message( "${EXTRA_LIB}")
target_link_libraries( "win" PRIVATE glad  ${EXTRA_LIB} /home/chh/Downloads/1.3.211.0/source/Vulkan-Loader/build/loader/libvulkan.so.1) 
set_target_properties(win PROPERTIES FOLDER "BlowRender/win" )
