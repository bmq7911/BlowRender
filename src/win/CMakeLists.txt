
project( "win" )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(GLOB_RECURSE WIN_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.inl )
file(GLOB_RECURSE WIN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c)


set( WIN_SRC ${WIN_HEADERS} ${WIN_SOURCES}  )

source_group_by_dir( WIN_SRC )
list( APPEND imgui_src "${CMAKE_SOURCE_DIR}/third/imgui/imgui/backends/imgui_impl_glfw.cpp" 
                       "${CMAKE_SOURCE_DIR}/third/imgui/imgui/backends/imgui_impl_opengl3.cpp"
                       "${CMAKE_SOURCE_DIR}/third/imgui/imgui/imgui.cpp"
                       "${CMAKE_SOURCE_DIR}/third/imgui/imgui/imgui_draw.cpp"
                       "${CMAKE_SOURCE_DIR}/third/imgui/imgui/imgui_tables.cpp" 
                       "${CMAKE_SOURCE_DIR}/third/imgui/imgui/imgui_widgets.cpp"
)

add_library( "win" STATIC ${WIN_SRC} ${imgui_src} )
target_include_directories( "win" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/src/core" "${CMAKE_SOURCE_DIR}/third/glad" 
                                          "${CMAKE_SOURCE_DIR}/third/glm" "${CMAKE_SOURCE_DIR}/third/imgui")
if(MSVC)
    target_include_directories( "win" PRIVATE "${VULKAN_PATH}/Include")
endif(MSVC)


target_link_libraries( "win" PRIVATE glad  ${EXTRA_LIB} ${VULKAN_LIB} ) 
set_target_properties(win PROPERTIES FOLDER "BlowRender/win" )
