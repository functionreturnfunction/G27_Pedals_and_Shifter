#include "Pedals.h"

int Pedals::getGas()
{
  return analogRead(GAS_PIN);
}

int Pedals::getBrake()
{
  return analogRead(BRAKE_PIN);
}

int Pedals::getClutch()
{
  return analogRead(CLUTCH_PIN);
}
