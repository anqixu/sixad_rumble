cmake_minimum_required(VERSION 2.8.3)
project(sixad_rumble)

find_package(catkin REQUIRED COMPONENTS
  roscpp
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES sixad_rumble
  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  include
)

add_library(${PROJECT_NAME}
  src/RumbleEventDriver.cpp
)

add_executable(test_rumble src/test_rumble.cpp)
target_link_libraries(test_rumble ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} test_rumble
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h*"
#  PATTERN ".svn" EXCLUDE
)
