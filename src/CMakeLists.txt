add_subdirectory( core )
add_subdirectory( editor)
add_subdirectory( helper )
add_subdirectory( win)

add_library( BlowRender STATIC $<TARGET_OBJECTS:core> $<TARGET_OBJECTS:editor> $<TARGET_OBJECTS:helper> $<TARGET_OBJECTS:win> )
set_target_properties( BlowRender PROPERTIES FOLDER "BlowRender")
target_link_libraries( BlowRender PRIVATE glad glfw )
