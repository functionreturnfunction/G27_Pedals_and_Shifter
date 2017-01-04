#ifndef Shifter_h
#define Shifter_h

#if defined(__TEST__)
#include "ArduinoMock.h"
#else
#include "Arduino.h"
#endif

// SHIFTER PINS
//| DB9 | Original | Harness | Shifter | Description             | Pro Micro   |
//|   1 | Purple   | Purple  |       1 | Button Clock            | pin 0       |
//|   2 | Grey     | Blue    |       7 | Button Data             | pin 1       |
//|   3 | Yellow   | Yellow  |       5 | Button !CS & !PL (Mode) | pin 4       |
//|   4 | Orange   | Orange  |       3 | Shifter X axis          | pin 8  (A8) |
//|   5 | White    | White   |       2 | SPI input               |             |
//|   6 | Black    | Black   |       8 | GND                     | GND         |
//|   7 | Red      | Red     |       6 | +5V                     | VCC         |
//|   8 | Green    | Green   |       4 | Shifter Y axis          | pin 9 (A9)  |
//|   9 | Red      | Red     |       1 | +5V                     | VCC         |
#define SHIFTER_CLOCK_PIN  0
#define SHIFTER_DATA_PIN   1
#define SHIFTER_MODE_PIN   4
#define SHIFTER_X_PIN      8
#define SHIFTER_Y_PIN      9

// BUTTON DEFINITIONS
#define BUTTON_REVERSE         1

#define BUTTON_RED_CENTERRIGHT 4
#define BUTTON_RED_CENTERLEFT  5
#define BUTTON_RED_RIGHT       6
#define BUTTON_RED_LEFT        7
#define BUTTON_BLACK_TOP       8
#define BUTTON_BLACK_RIGHT     9
#define BUTTON_BLACK_LEFT      10
#define BUTTON_BLACK_BOTTOM    11
#define BUTTON_DPAD_RIGHT      12
#define BUTTON_DPAD_LEFT       13
#define BUTTON_DPAD_BOTTOM     14
#define BUTTON_DPAD_TOP        15

#define OUTPUT_BLACK_TOP       9
#define OUTPUT_BLACK_LEFT      10
#define OUTPUT_BLACK_RIGHT     11
#define OUTPUT_BLACK_BOTTOM    12
#define OUTPUT_DPAD_TOP        13
#define OUTPUT_DPAD_LEFT       14
#define OUTPUT_DPAD_RIGHT      15
#define OUTPUT_DPAD_BOTTOM     16
#define OUTPUT_RED_LEFT        17
#define OUTPUT_RED_CENTERLEFT  18
#define OUTPUT_RED_CENTERRIGHT 19
#define OUTPUT_RED_RIGHT       20

// MISC.
#define SIGNAL_SETTLE_DELAY 10

class Shifter
{
  void waitForSignalToSettle();

public:
  void getButtonStates(int *ret);
  void getShifterPosition(int *ret);
};

#endif
