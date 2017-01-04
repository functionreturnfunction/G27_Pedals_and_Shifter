#ifndef CalibrationHelper_h
#define CalibrationHelper_h

#include <stdbool.h>
#include "EEPROMAnything.h"

struct calibration_t
{
  int minThrottle, maxThrottle,
    minBrake, maxBrake,
    minClutch, maxClutch;

  int gate13,
    gate24,
    gate35,
    gate46,
    lowerY,
    upperY;

  bool usePedals,
    useShifter,
    invertBrake,
    invertSequentialToggle;
};

class CalibrationHelper
{
public:
  calibration_t values;

  void writeToMemory();
  void readFromMemory();
};

#endif
