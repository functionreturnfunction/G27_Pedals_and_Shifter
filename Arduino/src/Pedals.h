#ifndef Pedals_h
#define Pedals_h

#if defined(__TEST__)
#include "ArduinoMock.h"
#else
#include "Arduino.h"
#endif

// PEDAL PINS
//| DB9 | Original | Harness | Description | Pro Micro   |
//|   1 | Black    | Red     | +5v         | +5v         |
//|   2 | Orange   | Yellow  | Throttle    | pin 18 (A0) |
//|   3 | White    | White   | Brake       | pin 19 (A1) |
//|   4 | Green    | Green   | Clutch      | pin 20 (A2) |
//|   5 |          |         |             |             |
//|   6 | Red      | Black   | GND         | GND         |
//|   7 |          |         |             |             |
//|   8 |          |         |             |             |
//|   9 | Red      | Black   | GND         | GND         |
#define THROTTLE_PIN    18
#define BRAKE_PIN  19
#define CLUTCH_PIN 20

class Pedals
{
 public:
    int getThrottle();
    int getBrake();
    int getClutch();
};

#endif
