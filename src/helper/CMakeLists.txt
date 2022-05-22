project( "helper" )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB_RECURSE HELPER_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.inl )
file(GLOB_RECURSE HELPER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c)



set( HELPER_SRC ${HELPER_HEADERS} ${HELPER_SOURCES}  )

source_group_by_dir( HELPER_SRC )

add_library( "helper" STATIC ${HELPER_SRC} )
target_include_directories( "helper" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/src/core" )
target_include_directories( "helper" PRIVATE "${CMAKE_SOURCE_DIR}/third/glm" "${CMAKE_SOURCE_DIR}/third" "${CMAKE_SOURCE_DIR}/MyLibs")
set_target_properties(helper PROPERTIES FOLDER "BlowRender/helper" )
