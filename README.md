sixad_rumble
============

Thin Linux input event driver, specifically targetting rumble support for wired PS3 gamepad / wireless PS3 gamepad + sixad.
C++ & Python versions.

## Compile C++ Wrapper without ROS:

1. copy include/[sixad_rumble/RumbleEventDriver.hpp], src/[RumbleEventDriver.cpp], src/[test_rumble.cpp] into your workspace (or a common folder; note the sixad_rumble subfolder)
2. gcc -o test_rumble RumbleEventDriver.cpp test_rumble.cpp

## Running C++ Test Code with ROS:
rosrun sixad_rumble test_rumble /dev/input/event00

## Running Python Test Code:
- ./nodes/RumbleEventDriver.py /dev/input/event00
- rosrun sixad_rumble RumbleEventDriver.py /dev/input/event00
