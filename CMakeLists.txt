cmake_minimum_required(VERSION 2.8)
project( ImageRegion )
find_package( OpenCV REQUIRED )
add_executable( ImageRegion ImageRegion.cpp )
target_link_libraries( ImageRegion ${OpenCV_LIBS} )
