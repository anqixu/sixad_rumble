cmake_minimum_required(VERSION 2.8.3)
project(sixad_rumble)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")

find_package(catkin REQUIRED COMPONENTS
  roscpp sensor_msgs
)

catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES sixad_rumble
  CATKIN_DEPENDS roscpp rospy sensor_msgs
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  include
)

add_executable(test_rumble src/RumbleEventDriver.cpp src/test_rumble.cpp)

add_executable(rumble_event_node src/RumbleEventNode.cpp)
target_link_libraries(rumble_event_node ${catkin_LIBRARIES})
