
project( "win" )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(GLOB_RECURSE WIN_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.inl )
file(GLOB_RECURSE WIN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c)


set( WIN_SRC ${WIN_HEADERS} ${WIN_SOURCES}  )

source_group_by_dir( WIN_SRC )

add_library( "win" STATIC ${WIN_SRC} )
target_include_directories( "win" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/src/core" "${CMAKE_SOURCE_DIR}/third/glad" 
                                          "${CMAKE_SOURCE_DIR}/third/glm")
if(MSVC)
    target_include_directories( "win" PRIVATE "${VULKAN_PATH}/Include")
endif(MSVC)


target_link_libraries( "win" PRIVATE glad  ${EXTRA_LIB} ${VULKAN_LIB}) 
set_target_properties(win PROPERTIES FOLDER "BlowRender/win" )
