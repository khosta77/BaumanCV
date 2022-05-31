cmake_minimum_required(VERSION 3.16)
project(main)

set(CMAKE_CXX_STANDARD 14)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(main
        project/src/main.cpp
        project/include/CV_two_array.h
        project/include/two_array.h
)
target_link_libraries( main ${OpenCV_LIBS} )