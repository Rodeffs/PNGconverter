# This is an example file that should be edited to fit the needs
# To build project using cmake type the following command in terminal: `cmake /path_to_source`
# It will build the project in the current folder. In the end type: `cmake --build .` to make it executable

cmake_minimum_required(VERSION 3.29) # cmake version, without it cmake simply won't run
project(PNGconverter) # write the name of the project in the brackets

set(CMAKE_CXX_STANDARD 17) # setting the standard of C++, should be 17 by default

find_package(PNG REQUIRED)

include_directories(${PNG_INCLUDE_DIR})


add_executable(PNGconverter inc/pngUtils.hpp src/pngUtils.cpp inc/Converter.hpp src/Converter.cpp src/main.cpp) # all files used (including the project name) must be written in the brackets

target_link_libraries(PNGconverter ${PNG_LIBRARY})
