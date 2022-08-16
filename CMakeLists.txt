cmake_minimum_required(VERSION 3.16)
project(main)

set(CMAKE_CXX_STANDARD 14)

#find_package( OpenCV REQUIRED )
#include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(main
        project/src/main.cpp
        project/include/CV_two_array.h
        project/include/SMatrix.h
        project/include/two_array.h
        project/include/utils.h
        project/src/utils.cpp
        project/include/correlation.h
)
#target_link_libraries( main ${OpenCV_LIBS} )

find_package(JPEG REQUIRED)
target_link_libraries(main PRIVATE ${JPEG_LIBRARIES})
target_include_directories(main PRIVATE ${JPEG_INCLUDE_DIR})