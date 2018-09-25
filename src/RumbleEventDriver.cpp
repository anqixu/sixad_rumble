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


bool RumbleEventDriver::debugFn() {
  if (rumble_fd < 0) return false;

  struct ff_effect re;
  struct input_event ev;

  // Define+upload+trigger+remove feedback effect 1
  memset(&re, 0, sizeof(re)); re.type = FF_RUMBLE; re.id = -1; re.replay.delay = 0;
  re.u.rumble.strong_magnitude = ((unsigned int) 255) << 8;
  re.u.rumble.weak_magnitude = ((unsigned int) 0) << 8;
  re.replay.length = 3000;
  if (ioctl(rumble_fd, EVIOCSFF, &re) == -1) {
    perror("Upload rumble ev");
    return false;
  }
  
  memset(&ev, 0, sizeof(ev)); ev.type = EV_FF; ev.code = re.id; ev.value = true;
  if (write(rumble_fd, (const void*) &ev, sizeof(ev)) == -1) {
    perror("Set rumble");
    return false;
  }

  sleep(5);
  
  ev.value = false;
  if (write(rumble_fd, (const void*) &ev, sizeof(ev)) == -1) {
    perror("Reset rumble");
    return false;
  }

  if (ioctl(rumble_fd, EVIOCRMFF, re.id) == -1) {
    perror("Clear rumble ev");
    return false;
  }
  
  // Define+upload+trigger+remove feedback effect 2
  memset(&re, 0, sizeof(re)); re.type = FF_RUMBLE; re.id = -1; re.replay.delay = 0;
  re.u.rumble.strong_magnitude = ((unsigned int) 0) << 8;
  re.u.rumble.weak_magnitude = ((unsigned int) 255) << 8;
  re.replay.length = 3000;
  if (ioctl(rumble_fd, EVIOCSFF, &re) == -1) {
    perror("Upload rumble ev");
    return false;
  }
  
  memset(&ev, 0, sizeof(ev)); ev.type = EV_FF; ev.code = re.id; ev.value = true;
  if (write(rumble_fd, (const void*) &ev, sizeof(ev)) == -1) {
    perror("Set rumble");
    return false;
  }

  sleep(5);
  
  ev.value = false;
  if (write(rumble_fd, (const void*) &ev, sizeof(ev)) == -1) {
    perror("Reset rumble");
    return false;
  }

  if (ioctl(rumble_fd, EVIOCRMFF, re.id) == -1) {
    perror("Clear rumble ev");
    return false;
  }
  
  // Define+upload+trigger+remove feedback effect 3
  memset(&re, 0, sizeof(re)); re.type = FF_RUMBLE; re.id = -1; re.replay.delay = 0;
  re.u.rumble.strong_magnitude = ((unsigned int) 128) << 8;
  re.u.rumble.weak_magnitude = ((unsigned int) 64) << 8;
  re.replay.length = 3000;
  if (ioctl(rumble_fd, EVIOCSFF, &re) == -1) {
    perror("Upload rumble ev");
    return false;
  }
  
  memset(&ev, 0, sizeof(ev)); ev.type = EV_FF; ev.code = re.id; ev.value = true;
  if (write(rumble_fd, (const void*) &ev, sizeof(ev)) == -1) {
    perror("Set rumble");
    return false;
  }

  sleep(5);
  
  ev.value = false;
  if (write(rumble_fd, (const void*) &ev, sizeof(ev)) == -1) {
    perror("Reset rumble");
    return false;
  }

  if (ioctl(rumble_fd, EVIOCRMFF, re.id) == -1) {
    perror("Clear rumble ev");
    return false;
  }

  return true;
};
