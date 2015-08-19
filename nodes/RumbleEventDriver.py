#! /usr/bin/env python

# This code is heavily based on: https://gitorious.org/shr/linux/commit/6d9826808441a773129fc93e189195ddaf040a4d


import sys
import getopt
import fcntl, struct, array


class RumbleEventDriver:
  def __init__(self):
    self.rumble_fd = None
    self.rumble_effect = None
    self.rumble_effect_array = None
    self.rumble_effect_id = None
    self.event_play = None
    self.event_stop = None


  def __del__(self):
    self.termRumble()

  # Opens device event file and uploads effect event
  # @arg strongMag, weakMag: 0x0: no rumble; 0x10: ~weakest rumble; 0xFF: strongest rumble
  # @arg durationMS: rumble duration, in milliseconds
  # @arg delayMS: event trigger delay, in milliseconds
  # @return file handle; returns -1 if failed to open device
  def initRumble(self, dev, strongMag=0xFF, weakMag=0x00, durationMS=5000, delayMS=0):
    # Open device
    self.termRumble()
    self.rumble_fd = open(dev, "r+") # raises IOError if dev file cannot be opened
    
    # Set and upload feedback effect ('struct ff_effect' from "linux/input.h")
    self.rumble_effect = struct.pack('HhHHHHHxxHH',
      0x50, -1, 0, # FF_RUMBLE, id, direction
      0, 0,        # trigger (button, interval)
      durationMS, delayMS,
      int((strongMag << 8) & 0x0FFFF), int((weakMag << 8) & 0x0FFFF))
    self.rumble_effect_array = array.array('h', self.rumble_effect)
    fcntl.ioctl(self.rumble_fd, 0x40304580, self.rumble_effect_array, True) # EVIOCSFF -> _IOC(_IOC_WRITE, 'E', 0x80, sizeof(struct ff_effect))
    # NOTE: EVIOCSFF is different from original gitorous code (0x402c4580)
    # - (_IOC_WRITE | ((sizeof(struct ff_effect) & IOCPARM_MASK) << 16) | (('E') << 8) | (0x80))
    # - _IOC_WRITE == 1U or 4U (different linux headers define this differently)
    # - 'E' in hex: 0x45
    # - IOCPARM_MASK = 0x1FFF

    self.rumble_effect_id = self.rumble_effect_array[1]

    # Set play and stop event structs ('struct input_event': sec, nsec, type, code, value)
    self.event_play = struct.pack('LLHHi', 0, 0, 0x15, self.rumble_effect_id, 1)
    self.event_stop = struct.pack('LLHHi', 0, 0, 0x15, self.rumble_effect_id, 0)
    
    return self.rumble_fd


  def termRumble(self):
    if self.rumble_fd is not None:
      self.setRumble(False)
      self.rumble_fd.close()
    self.rumble_fd = None
    self.rumble_effect = None
    self.rumble_effect_array = None
    self.rumble_effect_id = None
    self.event_play = None
    self.event_stop = None
    return True


  def setRumble(self, active):
    if self.rumble_fd is None:
      return False
    
    if active:
      self.rumble_fd.write(self.event_play)
    else:
      self.rumble_fd.write(self.event_stop)
    self.rumble_fd.flush()
    return True


def main():
  # Parse event device argument
  device_file_name = "/dev/input/gamepads/event_dft"
  argv = sys.argv
  for i in xrange(1, len(argv)):
    if argv[i] == "--help":
      print "Usage: %s /dev/input/eventXX" % argv[0]
      return 0
    else:
      device_file_name = argv[i]

  ps3 = RumbleEventDriver()
  ps3.initRumble(device_file_name)
  print "Device %s opened" % device_file_name
  
  # Prompt for user input
  i = 0
  try:
    while i >= 0:
      print ">0: rumble; 0: stop; <0: exit"
      i = int(raw_input())
      if i >= 0:
        if not ps3.setRumble(i > 0):
          return -1
  except ValueError:
    pass

  # Close rumble event driver
  ps3.termRumble()
  return 0


if __name__ == "__main__":
  sys.exit(main())
