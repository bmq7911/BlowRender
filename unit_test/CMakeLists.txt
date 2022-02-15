cmake_minimum_required(VERSION 3.00)
project( BlowRender_unit_test VERSION 1.0 )


add_executable( BlowRender_unit_test main.cpp test_ray_aabb.cpp SutherlandHodgman.cpp )
target_include_directories( BlowRender_unit_test PUBLIC "${CMAKE_CURRNET_SOURCE_DIR}/src")
target_include_directories( BlowRender_unit_test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../third/gtest")

target_link_libraries( BlowRender_unit_test  BlowRender )
target_link_libraries( BlowRender_unit_test "${CMAKE_CURRENT_SOURCE_DIR}/../libs/gtest/gtestd.lib")
set_target_properties( BlowRender_unit_test PROPERTIES FOLDER "UnitTest" )
