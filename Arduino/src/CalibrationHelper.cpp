#include "CalibrationHelper.h"

void CalibrationHelper::writeToMemory()
{
  EEPROM_writeAnything(0, values);
}

void CalibrationHelper::readFromMemory()
{
  EEPROM_readAnything(0, values);
}
