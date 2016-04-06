// G27_Pedals_and_Shifter.ino
// by Jason Duncan

// Partially adapted from the work done by isrtv.com forums members pascalh and xxValiumxx:
// http://www.isrtv.com/forums/topic/13189-diy-g25-shifter-interface-with-h-pattern-sequential-and-handbrake-modes/

#include <HID.h>
#include "./lib/G27PedalsShifter.h"

// for debugging, gives serial output rather than working as a joystick
//#define DEBUG true

#if defined(DEBUG)
#define DEBUG_PEDALS true
#define DEBUG_SHIFTER true
#endif

//#define DEBUG_PEDALS true
//#define DEBUG_SHIFTER true

// red for brake, green for gas, blue for clutch
//#define PEDAL_COLORS true

// use static thresholds rather than on-the-fly calibration
#define STATIC_THRESHOLDS true

// LED PINS
#define RED_PIN    3
#define GREEN_PIN  5
#define BLUE_PIN   6

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
#define GAS_PIN    18
#define BRAKE_PIN  19
#define CLUTCH_PIN 20

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

#define OUTPUT_BLACK_TOP       7
#define OUTPUT_BLACK_LEFT      8
#define OUTPUT_BLACK_RIGHT     9
#define OUTPUT_BLACK_BOTTOM    10
#define OUTPUT_DPAD_TOP        11
#define OUTPUT_DPAD_LEFT       12
#define OUTPUT_DPAD_RIGHT      13
#define OUTPUT_DPAD_BOTTOM     14
#define OUTPUT_RED_LEFT        15
#define OUTPUT_RED_CENTERLEFT  16
#define OUTPUT_RED_CENTERRIGHT 17
#define OUTPUT_RED_RIGHT       18

// SHIFTER AXIS THRESHOLDS
#define SHIFTER_XAXIS_12        330 //Gears 1,2
#define SHIFTER_XAXIS_56        620 //Gears 5,6, R
#define SHIFTER_YAXIS_135       600 //Gears 1,3,5
#define SHIFTER_YAXIS_246       300 //Gears 2,4,6, R

// PEDAL AXIS THRESHOLDS
#define MIN_GAS     27
#define MAX_GAS     925
#define MIN_BRAKE   26
#define MAX_BRAKE   845
#define MIN_CLUTCH  45
#define MAX_CLUTCH  932

// MISC.
#define MAX_AXIS            1023
#define SIGNAL_SETTLE_DELAY 10

// PEDAL CODE
typedef struct pedal {
  byte pin;
  int min, max, cur, axis;
};

typedef struct pedal Pedal;

void* gasPedal;
void* brakePedal;
void* clutchPedal;

int axisValue(void* in) {
  Pedal* input = (Pedal*)in;

  int physicalRange = input->max - input->min;
  if (physicalRange == 0) {
    return 0;
  }

  int result = map(input->cur, input->min, input->max, 0, MAX_AXIS);

  if (result < 0) {
    return 0;
  }
  if (result > MAX_AXIS) {
    return MAX_AXIS;
  }
  return result;
}

void processPedal(void* in) {
  Pedal* input = (Pedal*)in;

  input->cur = analogRead(input->pin);

  #if !defined(STATIC_THRESHOLDS)
  // calibrate, we want the highest this pedal has been
  input->max = input->cur > input->max ? input->cur : input->max;
  // same for lowest, but bottom out at current value rather than 0
  input->min = input->min == 0 || input->cur < input->min ? input->cur : input->min;
  #endif

  input->axis = axisValue(input);
}

void describePedal(char* name, char* axisName, void* in) {
  Pedal* input = (Pedal*)in;
  Serial.print("\nPIN: ");
  Serial.print(input->pin);
  Serial.print(" ");
  Serial.print(name);
  Serial.print(": ");
  Serial.print(input->cur);
  Serial.print(" MIN: ");
  Serial.print(input->min);
  Serial.print(" MAX: ");
  Serial.print(input->max);
  Serial.print(" ");
  Serial.print(axisName);
  Serial.print(" VALUE: ");
  Serial.print(input->axis);
}

void setXAxis(void* in) {
  Pedal* input = (Pedal*)in;
  G27.setXAxis(input->axis);
}

void setYAxis(void* in) {
  Pedal* input = (Pedal*)in;
  G27.setYAxis(input->axis);
}

void setZAxis(void* in) {
  Pedal* input = (Pedal*)in;
  G27.setZAxis(input->axis);
}

void pedalColor(void* inGas, void* inBrake, void* inClutch){
  Pedal* gas = (Pedal*)inGas;
  Pedal* brake = (Pedal*)inBrake;
  Pedal* clutch = (Pedal*)inClutch;

  setColor(brake->axis + MAX_AXIS, gas->axis + MAX_AXIS, clutch->axis + MAX_AXIS);
}

// SHIFTER CODE
int buttonTable[] = {
  // first four are unused
  0, 0, 0, 0,
  OUTPUT_RED_CENTERRIGHT,
  OUTPUT_RED_CENTERLEFT,
  OUTPUT_RED_RIGHT,
  OUTPUT_RED_LEFT,
  OUTPUT_BLACK_TOP,
  OUTPUT_BLACK_RIGHT,
  OUTPUT_BLACK_LEFT,
  OUTPUT_BLACK_BOTTOM,
  OUTPUT_DPAD_RIGHT,
  OUTPUT_DPAD_LEFT,
  OUTPUT_DPAD_BOTTOM,
  OUTPUT_DPAD_TOP
};

void waitForSignalToSettle() {
  delayMicroseconds(SIGNAL_SETTLE_DELAY);
}

void getButtonStates(int *ret) {
  digitalWrite(SHIFTER_MODE_PIN, LOW);    // Switch to parallel mode: digital inputs are read into shift register
  waitForSignalToSettle();
  digitalWrite(SHIFTER_MODE_PIN, HIGH);   // Switch to serial mode: one data bit is output on each clock falling edge

#if defined(DEBUG_SHIFTER)
  Serial.print("\nBUTTON STATES:");
#endif

  for(int i = 0; i < 16; ++i) {           // Iteration over both 8 bit registers
    digitalWrite(SHIFTER_CLOCK_PIN, LOW);         // Generate clock falling edge
    waitForSignalToSettle();

    ret[i] = digitalRead(SHIFTER_DATA_PIN);

#if defined(DEBUG_SHIFTER)
    if (!(i % 4)) Serial.print("\n");
    Serial.print(" button");
    if (i < 10) Serial.print(0);
    Serial.print(i);
    Serial.print(" = ");
    Serial.print(ret[i]);
#endif

    digitalWrite(SHIFTER_CLOCK_PIN, HIGH);        // Generate clock rising edge
    waitForSignalToSettle();
  }
}

void getShifterPosition(int *ret) {
  ret[0] = analogRead(SHIFTER_X_PIN);
  ret[1] = analogRead(SHIFTER_Y_PIN);
}

int getCurrentGear(int shifterPosition[], int btns[]) {
  int gear = 0;  // default to neutral
  int x = shifterPosition[0], y = shifterPosition[1];

    if (x < SHIFTER_XAXIS_12)                // Shifter on the left?
    {
      if (y > SHIFTER_YAXIS_135) gear = 1;   // 1st gear
      if (y < SHIFTER_YAXIS_246) gear = 2;   // 2nd gear
    }
    else if (x > SHIFTER_XAXIS_56)           // Shifter on the right?
    {
      if (y > SHIFTER_YAXIS_135) gear = 5;   // 5th gear
      if (y < SHIFTER_YAXIS_246) gear = 6;   // 6th gear
    }
    else                                     // Shifter is in the middle
    {
      if (y > SHIFTER_YAXIS_135) gear = 3;   // 3rd gear
      if (y < SHIFTER_YAXIS_246) gear = 4;   // 4th gear
    }

  if (gear != 6) btns[BUTTON_REVERSE] = 0;  // Reverse gear is allowed only on 6th gear position
  if (btns[BUTTON_REVERSE] == 1) gear = 7;  // Reverse is 7th gear (for the sake of argument)

  return gear;
}

void setButtonStates(int buttons[], int gear) {
  // release virtual buttons for all gears
  for (byte i = 0; i < 7; ++i) {
    G27.setButton(i, LOW);
  }

  if (gear > 0) {
    G27.setButton(gear - 1, HIGH);
  }

  for (byte i = BUTTON_RED_CENTERRIGHT; i <= BUTTON_DPAD_TOP; ++i) {
    G27.setButton(buttonTable[i], buttons[i]);
  }
}

void describeButtonStates(int buttons[], int shifterPosition[], int gear) {
  Serial.print("\nSHIFTER X: ");
  Serial.print(shifterPosition[0]);
  Serial.print(" Y: ");
  Serial.print(shifterPosition[1]);

  Serial.print(" GEAR: ");
  Serial.print(gear);
  Serial.print(" REVERSE: ");
  Serial.print(buttons[BUTTON_REVERSE]);

  Serial.print(" RED BUTTONS:");
  if (buttons[BUTTON_RED_LEFT]) {
    Serial.print(" 1");
  }
  if (buttons[BUTTON_RED_CENTERLEFT]) {
    Serial.print(" 2");
  }
  if (buttons[BUTTON_RED_CENTERLEFT]) {
    Serial.print(" 3");
  }
  if (buttons[BUTTON_RED_RIGHT]) {
    Serial.print(" 4");
  }

  Serial.print(" BLACK BUTTONS:");
  if (buttons[BUTTON_BLACK_LEFT]) {
    Serial.print(" LEFT");
  }
  if (buttons[BUTTON_BLACK_TOP]) {
    Serial.print(" TOP");
  }
  if (buttons[BUTTON_BLACK_BOTTOM]) {
    Serial.print(" BOTTOM");
  }
  if (buttons[BUTTON_BLACK_RIGHT]) {
    Serial.print(" RIGHT");
  }

  Serial.print(" D-PAD:");
  if (buttons[BUTTON_DPAD_LEFT]) {
    Serial.print(" LEFT");
  }
  if (buttons[BUTTON_DPAD_TOP]) {
    Serial.print(" UP");
  }
  if (buttons[BUTTON_DPAD_BOTTOM]) {
    Serial.print(" DOWN");
  }
  if (buttons[BUTTON_DPAD_RIGHT]) {
    Serial.print(" RIGHT");
  }
}

void setup() {
  Serial.begin(38400);
#if !defined(DEBUG_PEDALS) && !defined(DEBUG_SHIFTER)
  G27.begin(false);
#endif

  // lights
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // shifter
  pinMode(SHIFTER_MODE_PIN, OUTPUT);
  pinMode(SHIFTER_CLOCK_PIN, OUTPUT);

  digitalWrite(SHIFTER_MODE_PIN, HIGH);
  digitalWrite(SHIFTER_CLOCK_PIN, HIGH);

  // pedals
  Pedal* gas = new Pedal();
  Pedal* brake = new Pedal();
  Pedal* clutch = new Pedal();

  gas->pin = GAS_PIN;
  brake->pin = BRAKE_PIN;
  clutch->pin = CLUTCH_PIN;

  #if defined(STATIC_THRESHOLDS)
  gas->min = MIN_GAS;
  gas->max = MAX_GAS;

  brake->min = MIN_BRAKE;
  brake->max = MAX_BRAKE;

  clutch->min = MIN_CLUTCH;
  clutch->max = MAX_CLUTCH;
  #endif

  gasPedal = gas;
  brakePedal = brake;
  clutchPedal = clutch;
}

void loop() {
  // pedals
  processPedal(gasPedal);
  processPedal(brakePedal);
  processPedal(clutchPedal);

#if defined(DEBUG_PEDALS)
  describePedal("GAS", "X", gasPedal);
  describePedal("BRAKE", "Y", brakePedal);
  describePedal("CLUTCH", "Z", clutchPedal);
#else
  setXAxis(gasPedal);
  setYAxis(brakePedal);
  setZAxis(clutchPedal);
#endif

#if defined(PEDAL_COLORS)
  pedalColor(gasPedal, brakePedal, clutchPedal);
#endif

  // shifter
  int buttonStates[16];
  getButtonStates(buttonStates);
  int shifterPosition[2];
  getShifterPosition(shifterPosition);
  int gear = getCurrentGear(shifterPosition, buttonStates);

#if defined(DEBUG_SHIFTER)
  describeButtonStates(buttonStates, shifterPosition, gear);
#else
  setButtonStates(buttonStates, gear);
  G27.sendState();
#endif

#if defined(DEBUG_PEDALS) || defined(DEBUG_SHIFTER)
  Serial.print("\n----------------------------------------------------------------------------");
  // slow the output down a bit
  delay(500);
#endif
}

void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}
