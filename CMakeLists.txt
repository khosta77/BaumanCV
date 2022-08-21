cmake_minimum_required(VERSION 3.16)
project(main)

set(CMAKE_CXX_STANDARD 14)

add_executable(main
        project/src/main.cpp
        project/include/Mat.h
        project/include/SMatrix.h
        project/include/utils.h
        project/src/utils.cpp
        project/include/correlation.h
)

find_package(JPEG REQUIRED)
target_link_libraries(main PRIVATE ${JPEG_LIBRARIES})
target_include_directories(main PRIVATE ${JPEG_INCLUDE_DIR})

# Link against wiringPi
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
target_link_libraries(OpenInsulin ${WIRINGPI_LIBRARIES})