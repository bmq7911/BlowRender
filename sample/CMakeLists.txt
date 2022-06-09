cmake_minimum_required(VERSION 3.00)
project( drawLine)

add_executable( drawLine main.cpp Rasterizate.cpp RayTrace.cpp  )
target_include_directories( drawLine PRIVATE "${CMAKE_CURRNET_SOURCE_DIR}"
                                             "${CMAKE_SOURCE_DIR}/src/core"
                                             "${CMAKE_SOURCE_DIR}/src"
                                             "${CMAKE_SOURCE_DIR}/third/imgui"
                                             "${CMAKE_SOURCE_DIR}/third/glm"
                                             "${CMAKE_SOURCE_DIR}/third/glad")

target_link_libraries( drawLine PRIVATE BlowRender ${EXTRA_LIB} ${VULKAN_LIB})
set_target_properties( drawLine PROPERTIES FOLDER "Sample" )
