// Code derived from: https://github.com/flosse/linuxconsole/blob/master/utils/fftest.c

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "sixad_rumble/RumbleEventDriver.hpp"


RumbleEventDriver::RumbleEventDriver() : rumble_fd(-1) {
  // Define feedback effect
  memset(&rumble_effect, 0, sizeof(rumble_effect));
  rumble_effect.type = FF_RUMBLE;
  rumble_effect.id = -1; // Will get populated by ioctl
};


RumbleEventDriver::~RumbleEventDriver() {
  termRumble();
};


int RumbleEventDriver::initRumble(const char* dev,
    unsigned char strongMag, unsigned char weakMag,
    unsigned short durationMS, unsigned int delayMS) {
  // Open device
  rumble_fd = open(dev, O_RDWR);
  if (rumble_fd == -1) {
    perror("Open device file");
    return rumble_fd;
  }

  // Set and upload feedback effect
  rumble_effect.u.rumble.strong_magnitude = ((unsigned int) strongMag) << 8;
  rumble_effect.u.rumble.weak_magnitude = ((unsigned int) weakMag) << 8;
  rumble_effect.replay.length = durationMS;
  rumble_effect.replay.delay = delayMS;
  if (ioctl(rumble_fd, EVIOCSFF, &rumble_effect) == -1) {
    perror("Upload rumble event");
  }
  
  return rumble_fd;
};


bool RumbleEventDriver::setRumble(bool active) {
  if (rumble_fd < 0) return false;
  
  struct input_event event;
  memset(&event, 0, sizeof(event));
  event.type = EV_FF;
  event.code = rumble_effect.id;
  event.value = active;
  if (write(rumble_fd, (const void*) &event, sizeof(event)) == -1) {
    perror("Set rumble");
    return false;
  }
  
  return true;
};


bool RumbleEventDriver::termRumble() {
  if (rumble_fd >= 0) {
    setRumble(false);
    close(rumble_fd);
    rumble_fd = -1;
  }
  return true;
};
