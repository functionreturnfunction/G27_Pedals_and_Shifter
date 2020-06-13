// G27_Pedals_and_Shifter.ino
// by Jason Duncan

// Partially adapted from the work done by isrtv.com forums members pascalh and xxValiumxx:
// http://www.isrtv.com/forums/topic/13189-diy-g25-shifter-interface-with-h-pattern-sequential-and-handbrake-modes/

#include <HID.h>
#include <EEPROM.h>
#include "./src/G27PedalsShifter.h"

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

// MISC.
#define MAX_AXIS            1023
#define SIGNAL_SETTLE_DELAY 10

#define CALIB_DATA_MAGIC_NUMBER 0x27CA11B1 // change this when the struct definition changes

/* this structure holds the calibration data stored in EEPROM */
typedef struct calibration
{
    struct {
        /* magic number, used for testing valid EEPROM content */
        int32_t calibID;
        /* bool whether to automatically calibrate the pedals at a power cycle or use a static calibration */
        uint8_t pedals_auto_calib; 
        /* bool whether to invert the brake pedal */
        uint8_t invert_brake;
        /* bool whether to use the pedals */
        uint8_t use_pedals;
        /* bool whether to use the shifter */
        uint8_t use_shifter;
        /* only meaningful if pedals_auto_calib is 0 */
        uint16_t gasMin, gasMax, brakeMin, brakeMax, clutchMin, clutchMax;
        /* defines the neutral zone of the y shifter hysteresis */
        uint16_t shifter_y_neutralMin, shifter_y_neutralMax;
        /* defines the 246R zone of the y shifter hysteresis, should be less than shifter_y_neutralMin */
        uint16_t shifter_y_246R_gearZone;
        /* defines the 135 zone of the y shifter hysteresis, should be greater than shifter_y_neutralMax */
        uint16_t shifter_y_135_gearZone;
        /* threshold for gears 1 and 2 of the x shifter, should be less than shifter_x_56 */
        uint16_t shifter_x_12;
        /* threshold for gears 5,6 and R of the x shifter, should be greater than shifter_x_12 */
        uint16_t shifter_x_56;
    } data;
    /* CRC checksum */
    uint32_t crc;
} Calibration;

static Calibration calibDefault = {
    {
        CALIB_DATA_MAGIC_NUMBER,
        1, /* pedals auto calib */
        0, /* invert brake */
        1, /* use pedals */
        1, /* use shifter */
        0,0,0,0,0,0, /* calibrated pedal values */
        1024/2 - 1024/10, 1024/2 + 1024/10, /* shifter_y_neutral */
        1024/2 - 1024/3, 1024/2 + 1024/3,   /* shifter_y_gearZone */
        1024/3, 1024*2/3 /* shifter_x */
    },
    0 /* crc will be set when written to EEPROM */
};
static Calibration calibration = calibDefault;

unsigned long crc(uint8_t *buffer, uint16_t length) {

  const unsigned long crc_table[16] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
  };

  unsigned long crc = ~0L;

  for (int index = 0 ; index < length ; ++index) {
    crc = crc_table[(crc ^ buffer[index]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (buffer[index] >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }
  return crc;
}

static int crcError = 0;
static int magicNumberError = 0;
static int lastCrcFromEEPROM = 0;
static int lastCrcFromContents = 0;
void loadEEPROM()
{
  Calibration fromEEPROM;
  EEPROM.get(0, fromEEPROM);
  
  uint32_t eeprom_crc = crc((uint8_t*)&fromEEPROM.data, sizeof(fromEEPROM.data));
  lastCrcFromEEPROM = fromEEPROM.crc;
  lastCrcFromContents = eeprom_crc;
  if( eeprom_crc == fromEEPROM.crc && fromEEPROM.data.calibID == CALIB_DATA_MAGIC_NUMBER )
  {
    calibration = fromEEPROM;
  } else
  {
    crcError += eeprom_crc != fromEEPROM.crc;
    magicNumberError += fromEEPROM.data.calibID != CALIB_DATA_MAGIC_NUMBER;
    calibration = calibDefault;
  }
}

// PEDAL CODE
typedef struct Pedal {
  byte pin;
  int min, max, cur, axis;
} Pedal;

static Pedal* gasPedal = 0;
static Pedal* brakePedal = 0;
static Pedal* clutchPedal = 0;

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

  if(calibration.data.pedals_auto_calib)
  {
    // calibrate, we want the highest this pedal has been
    input->max = input->cur > input->max ? input->cur : input->max;
    // same for lowest, but bottom out at current value rather than 0
    input->min = input->min == 0 || input->cur < input->min ? input->cur : input->min;
  }

  input->axis = axisValue(input);
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

  for(int i = 0; i < 16; ++i) {           // Iteration over both 8 bit registers
    digitalWrite(SHIFTER_CLOCK_PIN, LOW);         // Generate clock falling edge
    waitForSignalToSettle();

    ret[i] = digitalRead(SHIFTER_DATA_PIN);

    digitalWrite(SHIFTER_CLOCK_PIN, HIGH);        // Generate clock rising edge
    waitForSignalToSettle();
  }
}

void getShifterPosition(int *ret) {
  ret[0] = analogRead(SHIFTER_X_PIN);
  ret[1] = analogRead(SHIFTER_Y_PIN);
}

int getCurrentGear(int shifterPosition[], int btns[]) {
  static int gear = 0;  // default to neutral
  int x = shifterPosition[0], y = shifterPosition[1];

  if (y < calibration.data.shifter_y_neutralMax && y > calibration.data.shifter_y_neutralMin)
  {
    gear = 0;
  }

  if (y > calibration.data.shifter_y_135_gearZone )
  {
    if( x <= calibration.data.shifter_x_12 )
    {
      if( gear != 3 ) /* avoid toggles between neighboring gears */
      {
        gear = 1;
      }
    } else if ( x >= calibration.data.shifter_x_56 )
    {
      if( gear != 3 ) /* avoid toggles between neighboring gears */
      {
        gear = 5;
      }
    } else
    {
      if( gear != 1 && gear != 5 ) /* avoid toggles between neighboring gears */
      {
        gear = 3;
      }
    }
  }
  if (y < calibration.data.shifter_y_246R_gearZone )
  {
    if( x <= calibration.data.shifter_x_12 )
    {
      if( gear != 4 ) /* avoid toggles between neighboring gears */
      {
        gear = 2;
      }
    } else if ( x >= calibration.data.shifter_x_56 )
    {
      if( btns[BUTTON_RED_RIGHT] ) /* hack for broken revers sensor */
      {
        if( gear != 4 && gear != 6 ) /* avoid toggles between neighboring gears */
        {
          gear = 7;
        }
      } else
      {
        if( gear != 4 && gear != 7) /* avoid toggles between neighboring gears */
        {
          gear = 6;
        }
      }
    } else
    {
      if( gear != 2 && gear != 6  && gear != 7) /* avoid toggles between neighboring gears */
      {
        gear = 4;
      }
    }
  }

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

#define CALIB_MIN(calibValue, curValue, minValue) if( calibValue < 0 || curValue < minValue ) { calibValue = minValue = curValue; }
#define CALIB_MAX(calibValue, curValue, maxValue) if( calibValue < 0 || curValue > maxValue ) { calibValue = maxValue = curValue; }
#define CALIB_RANGE(calibValue, curValue, minValue, maxValue) \
    { if( calibValue < 0 ) { calibValue = minValue = maxValue = curValue; } \
      CALIB_MIN(calibValue, curValue, minValue); \
      CALIB_MAX(calibValue, curValue, maxValue); }

void calib(struct Pedal *gas, Pedal *brake, Pedal *clutch, int shifter_X, int shifter_Y, int calibButton) 
{
  static uint8_t printMode = 0;
  static enum COMMANDS {
      IDLE = 'i',
      RECORD_Y_SHIFTER_135 = 'u',
      RECORD_Y_SHIFTER_246 = 'b',
      RECORD_Y_SHIFTER_NEUTRAL = 'n',
      RECORD_X_SHIFTER_12 = 'l',
      RECORD_X_SHIFTER_56 = 'r',
      RECORD_GAS = 'G',
      RECORD_BRAKE = 'B',
      RECORD_CLUTCH = 'C',
      SET_PEDAL_AUTO_CALIBRATE = 'P',
      RESET_PEDAL_AUTO_CALIBRATE = 'p',
      SET_PEDALS_ENABLED = 'E',
      RESET_PEDALS_ENABLED = 'e',
      SET_SHIFTER_ENABLED = 'S',
      RESET_SHIFTER_ENABLED = 's',
      SET_BRAKE_INVERTED = 'X',
      RESET_BRAKE_INVERTED = 'x',
      SET_PRINT_MODE = 'O',
      RESET_PRINT_MODE = 'o',
      STORE_CALIB = 'w',
      DEFAULT_CALIB = 'c',
      EEPROM_CALIB = 'U'
  } currentMode = IDLE;
  static int calibValue = -1;
  if (Serial.available() > 0) 
  {
    char rx_byte = Serial.read();
    switch (rx_byte)
    {
      case IDLE:
      case RECORD_Y_SHIFTER_135:        
      case RECORD_Y_SHIFTER_246:
      case RECORD_Y_SHIFTER_NEUTRAL:
      case RECORD_X_SHIFTER_12:
      case RECORD_X_SHIFTER_56:
      case RECORD_GAS:
      case RECORD_BRAKE:
      case RECORD_CLUTCH: calibValue = -1; currentMode = (COMMANDS)rx_byte; break;
      case SET_PEDAL_AUTO_CALIBRATE: calibration.data.pedals_auto_calib = 1; break;
      case RESET_PEDAL_AUTO_CALIBRATE: calibration.data.pedals_auto_calib= 0; break;
      case SET_PEDALS_ENABLED: calibration.data.use_pedals = 1; break;
      case RESET_PEDALS_ENABLED: calibration.data.use_pedals = 0; break;
      case SET_SHIFTER_ENABLED: calibration.data.use_shifter = 1; break;
      case RESET_SHIFTER_ENABLED: calibration.data.use_shifter = 0; break;
      case SET_BRAKE_INVERTED: calibration.data.invert_brake = 1; break;
      case RESET_BRAKE_INVERTED: calibration.data.invert_brake = 0; break;
      case SET_PRINT_MODE: printMode = 1; break;
      case RESET_PRINT_MODE: printMode = 0;; break;
      case STORE_CALIB:
        calibration.crc = crc((uint8_t*)&calibration.data, sizeof(calibration.data));
        EEPROM.put(0, calibration);
        break;
      case DEFAULT_CALIB:
        calibration = calibDefault;
        break;
      case EEPROM_CALIB:
        loadEEPROM();
        break;
    }
  }
  if(printMode)
  {
    Serial.print(calibButton); Serial.print(" ");
    Serial.print(shifter_X); Serial.print(" ");
    Serial.print(shifter_Y); Serial.print(" ");
    Serial.print(gas->cur); Serial.print(" ");
    Serial.print(brake->cur); Serial.print(" ");
    Serial.print(clutch->cur); Serial.print(" ");
    Serial.print(calibration.data.shifter_y_135_gearZone); Serial.print(" ");
    Serial.print(calibration.data.shifter_y_246R_gearZone); Serial.print(" ");
    Serial.print(calibration.data.shifter_y_neutralMin); Serial.print(" ");
    Serial.print(calibration.data.shifter_y_neutralMax); Serial.print(" ");
    Serial.print(calibration.data.shifter_x_12); Serial.print(" ");
    Serial.print(calibration.data.shifter_x_56); Serial.print(" ");
    Serial.print(calibration.data.gasMin); Serial.print(" ");
    Serial.print(calibration.data.gasMax); Serial.print(" ");
    Serial.print(calibration.data.brakeMin); Serial.print(" ");
    Serial.print(calibration.data.brakeMax); Serial.print(" ");
    Serial.print(calibration.data.clutchMin); Serial.print(" ");
    Serial.print(calibration.data.clutchMax); Serial.print(" ");
    Serial.print(calibration.data.pedals_auto_calib); Serial.print(" ");
    Serial.print(calibration.data.use_pedals); Serial.print(" ");
    Serial.print(calibration.data.use_shifter); Serial.print(" ");
    Serial.print(calibration.data.invert_brake); Serial.print(" ");
    Serial.print(crcError); Serial.print(" ");
    Serial.print(magicNumberError); Serial.print(" ");
    Serial.print(lastCrcFromEEPROM); Serial.print(" ");
    Serial.print(lastCrcFromContents); Serial.print(" ");
    Serial.print("                    \n");
    delay(1); /* delay 1 ms */
  }
  if(calibButton)
  {
    switch(currentMode)
    {
      case RECORD_Y_SHIFTER_135:
        CALIB_MIN(calibValue, shifter_Y, calibration.data.shifter_y_135_gearZone);
        break;
      case RECORD_Y_SHIFTER_246:
        CALIB_MAX(calibValue, shifter_Y, calibration.data.shifter_y_246R_gearZone);
        break;
      case RECORD_Y_SHIFTER_NEUTRAL:
        CALIB_RANGE(calibValue, shifter_Y, calibration.data.shifter_y_neutralMin, calibration.data.shifter_y_neutralMax);
        break;
      case RECORD_X_SHIFTER_12:
        CALIB_MAX(calibValue, shifter_X, calibration.data.shifter_x_12);
        break;
      case RECORD_X_SHIFTER_56:
        CALIB_MIN(calibValue, shifter_X, calibration.data.shifter_x_56);
        break;
    }
  }
  switch(currentMode)
  {
    case RECORD_GAS:
      CALIB_RANGE(calibValue, gas->cur, calibration.data.gasMin, calibration.data.gasMax);
      break;
    case RECORD_BRAKE:
      CALIB_RANGE(calibValue, brake->cur, calibration.data.brakeMin, calibration.data.brakeMax);
      break;
    case RECORD_CLUTCH:         
      CALIB_RANGE(calibValue, clutch->cur, calibration.data.clutchMin, calibration.data.clutchMax);
      break;
  }
}

void setup() {
  Serial.begin(115200);

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

  loadEEPROM();

  if( !calibration.data.pedals_auto_calib )
  {
    gas->min = calibration.data.gasMin;
    gas->max = calibration.data.gasMax;

    brake->min = calibration.data.brakeMin;
    brake->max = calibration.data.brakeMax;

    clutch->min = calibration.data.clutchMin;
    clutch->max = calibration.data.clutchMax;
  } else
  {
    gas->min = brake->min = clutch->min = MAX_AXIS;
    gas->max = brake->max = clutch->max = 0;
  }

  gasPedal = gas;
  brakePedal = brake;
  clutchPedal = clutch;
}

void loop() {
  // pedals
  processPedal(gasPedal);
  processPedal(brakePedal);
  processPedal(clutchPedal);

  if(calibration.data.invert_brake )
  {
    Pedal* brake = (Pedal*)brakePedal;
    brake->axis = map(brake->axis, 0, MAX_AXIS, MAX_AXIS, 0);
  }

  if(calibration.data.use_pedals)
  {
    setXAxis(gasPedal);
    setYAxis(brakePedal);
    setZAxis(clutchPedal);
  }

  // shifter
  int buttonStates[16];
  getButtonStates(buttonStates);
  int shifterPosition[2];
  getShifterPosition(shifterPosition);
  int gear = getCurrentGear(shifterPosition, buttonStates);

  if(calibration.data.use_shifter)
  {
    setButtonStates(buttonStates, gear);
  }

  calib(gasPedal, brakePedal, clutchPedal, shifterPosition[0], shifterPosition[1], buttonStates[BUTTON_RED_LEFT]);
  G27.sendState();
}
