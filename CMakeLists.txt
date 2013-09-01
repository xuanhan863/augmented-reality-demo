cmake_minimum_required(VERSION 2.8)
project( track )
find_package( OpenCV REQUIRED )
find_package( Boost 1.40 COMPONENTS thread system REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )
add_executable( track src/main.cpp )
target_link_libraries( track
  ${OpenCV_LIBS}
  ${Boost_LIBRARIES} )
