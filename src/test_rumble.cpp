#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "sixad_rumble/RumbleEventDriver.hpp"


int main(int argc, char** argv) {
  // Parse event device argument
  int i;
  const char* dev = "/dev/input/gamepads/event_dft";
  for (i = 1; i < argc; i++) {
    if (strncmp(argv[i], "--help", 6) == 0) {
      printf("Usage: %s /dev/input/eventXX <strong_magn> <weak_magn>\n", argv[0]);
      return EXIT_SUCCESS;
    }
  }
  
  unsigned char strongMag = 0xFF, weakMag = 0x00;
  if (argc >= 2) dev = argv[1];
  if (argc >= 3) strongMag = atoi(argv[2]);
  if (argc >= 4) weakMag = atoi(argv[3]);
  
  RumbleEventDriver ps3;

  // Initialize rumble event driver
  if (ps3.initRumble(dev, strongMag, weakMag) < 0) {
    return EXIT_FAILURE;
  }
  printf("Device %s opened\n", dev);
  

  // Prompt for user input
  do {
    printf(">0: rumble; 0: stop; <0: exit\n");
    i = -1;
    if (scanf("%d", &i) == EOF) {
      printf("Read error\n");
    } else if (i >= 0) {
      /*
      if (!ps3.setRumble(i > 0)) {
        return EXIT_FAILURE;
      }
      */
      if (!ps3.debugFn()) {
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
