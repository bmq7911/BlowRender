project( "core" )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)




file(GLOB_RECURSE CORE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.inl )
file(GLOB_RECURSE CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c)


set( CORE_SRC ${CORE_HEADERS} ${CORE_SOURCES}  )

source_group_by_dir( CORE_SRC )

add_library( "core" STATIC ${CORE_SRC} )
target_include_directories( "core" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}  "${CMAKE_SOURCE_DIR}/src" )
target_include_directories( "core" PRIVATE "${CMAKE_SOURCE_DIR}/third/glm" "${CMAKE_SOURCE_DIR}/third/glfw-3.2.1/include" "${CMAKE_SOURCE_DIR}/MyLibs")

set_target_properties("core" PROPERTIES FOLDER "BlowRender/core" )
