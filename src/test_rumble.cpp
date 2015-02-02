#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "sixad_rumble/RumbleEventDriver.hpp"


int main(int argc, char** argv) {
  // Parse event device argument
  int i;
  const char* device_file_name = "/dev/input/event0";
  for (i = 1; i < argc; i++) {
    if (strncmp(argv[i], "--help", 64) == 0) {
      printf("Usage: %s /dev/input/eventXX\n", argv[0]);
      return EXIT_SUCCESS;
    }
    else {
      device_file_name = argv[i];
    }
  }
  
  RumbleEventDriver ps3;

  // Initialize rumble event driver
  if (ps3.initRumble(device_file_name) < 0) {
    return EXIT_FAILURE;
  }
  printf("Device %s opened\n", device_file_name);
  

  // Prompt for user input
  do {
    printf(">0: rumble; 0: stop; <0: exit\n");
    i = -1;
    if (scanf("%d", &i) == EOF) {
      printf("Read error\n");
    } else if (i >= 0) {
      if (!ps3.setRumble(i > 0)) {
        return EXIT_FAILURE;
      }
    }
  } while (i >= 0);

  // Close rumble event driver
  if (!ps3.termRumble()) {
    perror("Close event file");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
};
