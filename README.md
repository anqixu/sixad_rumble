sixad_rumble
============

Thin Linux input event driver, specifically targetting rumble support for wired PS3 gamepad / wireless PS3 gamepad + sixad

This wrapper can be compiled and used without ROS:
1. copy include/[sixad_rumble/RumbleEventDriver.hpp], src/[RumbleEventDriver.cpp], src/[test_rumble.cpp] into your workspace (or a common folder; note the sixad_rumble subfolder)
2. gcc -o test_rumble RumbleEventDriver.cpp test_rumble.cpp
