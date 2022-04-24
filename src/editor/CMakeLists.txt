

project( "editor" )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


file(GLOB_RECURSE EDITOR_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.inl )
file(GLOB_RECURSE EDITOR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c)


set( EDITOR_SRC ${EDITOR_HEADERS} ${EDITOR_SOURCES} ${DEV_SCRIPT_RC} )

source_group_by_dir( EDITOR_SRC )

add_library( "editor" STATIC ${EDITOR_SRC} )
target_include_directories( "editor" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )
set_target_properties("editor" PROPERTIES FOLDER "BlowRender/editor" )

