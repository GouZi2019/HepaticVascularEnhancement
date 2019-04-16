
cmake_minimum_required(VERSION 2.8)
 

project(nice)
 

find_package(ITK REQUIRED)

include(${ITK_USE_FILE})

 

add_executable(nice MACOSX_BUNDLE nice.cxx)

target_link_libraries(nice ${ITK_LIBRARIES})
