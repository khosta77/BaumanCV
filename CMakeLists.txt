cmake_minimum_required(VERSION 3.16)
project(main)

set(CMAKE_CXX_STANDARD 14)

#find_package( OpenCV REQUIRED )
#include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(main
        project/src/main.cpp
        project/include/CV_two_array.h
        project/include/two_array.h
        project/include/utils.h
        project/src/utils.cpp
        project/include/correlation.h
        project/include/CV/
        project/include/CV/all_in.h
        project/include/CV/BaseImageDecoder.h
        project/include/CV/Enum_status.h
        project/include/CV/ExifEntry.h
        project/include/CV/Mat.h
        project/include/CV/Matx.h
        project/include/CV/Ptr.h
        project/include/CV/Scalar.h
        project/include/CV/Size.h
        project/include/CV/Size_validateInputImageSize.h
        project/include/CV/uchar_in_typedef.h
        project/include/CV/Vec.h
)
#target_link_libraries( main ${OpenCV_LIBS} )