#ifndef HardwareHelper_h
#define HardwareHelper_h

#include "Pedals.h"
#include "Shifter.h"

class HardwareHelper
{
protected:
  Pedals pedals;
  Shifter shifter;

public:
  void setup();
  int getThrottle();
  int getBrake();
  int getClutch();
  void getButtonStates(int *ret);
  void getShifterPosition(int *ret);
};

#endif
