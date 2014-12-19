#ifndef RUMBLEEVENTDRIVER_HPP_
#define RUMBLEEVENTDRIVER_HPP_


#include <linux/input.h>


/**
 * This is a thin wrapper for sending events to /dev/input/event#,
 * specifically aimed programmatically controlling rumble to PS3 gamepad via sixad.
 */
class RumbleEventDriver {
public:
  RumbleEventDriver();
  ~RumbleEventDriver();
  
  /**
   * Opens device event file and uploads effect event
   * 
   * @arg strongMag, weakMag: 0x0: no rumble; 0x10: ~weakest rumble; 0xFF: strongest rumble
   * @arg durationMS: rumble duration, in milliseconds
   * @arg delayMS: event trigger delay, in milliseconds
   * @return file handle; returns -1 if failed to open device
   */
  int initRumble(const char* dev, unsigned char strongMag = 0xFF, unsigned char weakMag = 0x00,
      unsigned short durationMS = 5000, unsigned int delayMS = 0);
  
  /**
   * Engages/disengages rumble event
   * 
   * @arg active: desired rumble state
   * @return True if success; False if failed or device not open
   */
  bool setRumble(bool active);

  /**
   * Stops rumble event and closes device event file
   * 
   * @return False if failed to close file descriptor
   */
  bool termRumble();

  
protected:
  int rumble_fd;
  struct ff_effect rumble_effect;
};


#endif // RUMBLEEVENTDRIVER_HPP_
