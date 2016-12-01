#include "HardwareHelper.h"

void HardwareHelper::setup()
{
  pinMode(SHIFTER_MODE_PIN, OUTPUT);
  pinMode(SHIFTER_CLOCK_PIN, OUTPUT);

  digitalWrite(SHIFTER_MODE_PIN, HIGH);
  digitalWrite(SHIFTER_CLOCK_PIN, HIGH);
}

int HardwareHelper::getGas()
{
  return pedals.getGas();
}

int HardwareHelper::getBrake()
{
  return pedals.getBrake();
}

int HardwareHelper::getClutch()
{
  return pedals.getClutch();
}

void HardwareHelper::getButtonStates(int* ret)
{
  shifter.getButtonStates(ret);
}

void HardwareHelper::getShifterPosition(int* ret)
{
  shifter.getShifterPosition(ret);
}
