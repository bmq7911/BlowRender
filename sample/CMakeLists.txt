cmake_minimum_required(VERSION 3.00)
project( drawLine)
add_compile_options(/wd4819)

add_definitions( -D _CRT_SECURE_NO_WARNINGS)

add_executable( drawLine main.cpp Rasterizate.cpp RayTrace.cpp  )
target_include_directories( drawLine PRIVATE "${CMAKE_CURRNET_SOURCE_DIR}"
                                             "${CMAKE_SOURCE_DIR}/src/core"
                                             "${CMAKE_SOURCE_DIR}/src"
                                             "${CMAKE_SOURCE_DIR}/third/glm"
                                             "${CMAKE_SOURCE_DIR}/third/glad"
                                             "${CMAKE_SOURCE_DIR}/third/glfw-3.2.1/include"  )
target_link_libraries( drawLine PRIVATE BlowRender )
set_target_properties( drawLine PROPERTIES FOLDER "Sample" )
