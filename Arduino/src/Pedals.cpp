#include "Pedals.h"

int Pedals::getThrottle()
{
  return analogRead(THROTTLE_PIN);
}

int Pedals::getBrake()
{
  return analogRead(BRAKE_PIN);
}

int Pedals::getClutch()
{
  return analogRead(CLUTCH_PIN);
}
