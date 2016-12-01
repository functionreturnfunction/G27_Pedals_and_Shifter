#include "Controller.h"

CmdMessenger Controller::cmdMessenger = CmdMessenger(Serial);
HardwareHelper Controller::hardware;
CalibrationHelper Controller::calibration;
Joystick Controller::joystick;

void Controller::attachCommandCallbacks()
{
  cmdMessenger.attach(onUnkownCommand);
  cmdMessenger.attach(Command::kAcknowledge, onAcknowledge);
  cmdMessenger.attach(Command::kGetStatus, onGetStatus);
  cmdMessenger.attach(Command::kGetCalibration, onGetCalibration);
  cmdMessenger.attach(Command::kSetCalibration, onSetCalibration);
}

static void Controller::onUnkownCommand()
{
  cmdMessenger.sendCmd(Command::kError, "Command without attached callback");
}

static void Controller::onAcknowledge()
{
  cmdMessenger.sendCmd(Command::kAcknowledge, "Why hello there");
}

static void Controller::onGetStatus()
{
  int buttonStates[16];
  int shifterPosition[2];

  hardware.getButtonStates(buttonStates);
  hardware.getShifterPosition(shifterPosition);

  cmdMessenger.sendCmdStart(Command::kGetStatusResult);

  cmdMessenger.sendCmdArg(hardware.getThrottle());
  cmdMessenger.sendCmdArg(hardware.getBrake());
  cmdMessenger.sendCmdArg(hardware.getClutch());

  for (int i = 0; i < 16; i++)
  {
    cmdMessenger.sendCmdArg(buttonStates[i]);
  }

  for (int i = 0; i < 2; i++)
  {
    cmdMessenger.sendCmdArg(shifterPosition[i]);
  }

  cmdMessenger.sendCmdEnd();
}

static void Controller::onGetCalibration()
{
  cmdMessenger.sendCmdStart(Command::kGetCalibrationResult);

  cmdMessenger.sendCmdArg(calibration.values.minThrottle);
  cmdMessenger.sendCmdArg(calibration.values.maxThrottle);
  cmdMessenger.sendCmdArg(calibration.values.minBrake);
  cmdMessenger.sendCmdArg(calibration.values.maxBrake);
  cmdMessenger.sendCmdArg(calibration.values.minClutch);
  cmdMessenger.sendCmdArg(calibration.values.maxClutch);
  cmdMessenger.sendCmdArg(calibration.values.gate13);
  cmdMessenger.sendCmdArg(calibration.values.gate24);
  cmdMessenger.sendCmdArg(calibration.values.gate35);
  cmdMessenger.sendCmdArg(calibration.values.gate46);
  cmdMessenger.sendCmdArg(calibration.values.lowerY);
  cmdMessenger.sendCmdArg(calibration.values.upperY);

  cmdMessenger.sendCmdEnd();
}

static void Controller::onSetCalibration()
{
  calibration.values.minThrottle = cmdMessenger.readInt16Arg();
  calibration.values.maxThrottle = cmdMessenger.readInt16Arg();
  calibration.values.minBrake = cmdMessenger.readInt16Arg();
  calibration.values.maxBrake = cmdMessenger.readInt16Arg();
  calibration.values.minClutch = cmdMessenger.readInt16Arg();
  calibration.values.maxClutch = cmdMessenger.readInt16Arg();

  calibration.values.gate13 = cmdMessenger.readInt16Arg();
  calibration.values.gate24 = cmdMessenger.readInt16Arg();
  calibration.values.gate35 = cmdMessenger.readInt16Arg();
  calibration.values.gate46 = cmdMessenger.readInt16Arg();
  calibration.values.lowerY = cmdMessenger.readInt16Arg();
  calibration.values.upperY = cmdMessenger.readInt16Arg();

  calibration.writeToMemory();
  cmdMessenger.sendCmd(Command::kSetCalibrationResult);
}

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

static int Controller::getCurrentGear(int *shifterPosition, int *btns)
{
  int gear = 0;  // default to neutral
  int x = shifterPosition[0], y = shifterPosition[1];

  if (y > calibration.values.lowerY)
  {
    if (x < calibration.values.gate13)
    {
      gear = 1;
    }
    else if (x > calibration.values.gate13 && x < calibration.values.gate35)
    {
      gear = 3;
    }
    else if (x > calibration.values.gate35)
    {
      gear = 5;
    }
  }
  else if (y < calibration.values.upperY)
  {
    if (x < calibration.values.gate24)
    {
      gear = 2;
    }
    else if (x > calibration.values.gate24 && x < calibration.values.gate46)
    {
      gear = 4;
    }
    else if (x > calibration.values.gate46)
    {
      gear = 6;
    }
  }

  if (gear != 6) btns[BUTTON_REVERSE] = 0;  // Reverse gear is allowed only on 6th gear position
  if (btns[BUTTON_REVERSE] == 1) gear = 7;  // Reverse is 7th gear (for the sake of argument)

  return gear;
}

static void Controller::updateJoystick()
{
  joystick.setXAxis(map(hardware.getThrottle(), 0, 1023, calibration.values.minThrottle, calibration.values.maxThrottle));
  joystick.setYAxis(map(hardware.getBrake(), 0, 1023, calibration.values.minBrake, calibration.values.maxBrake));
  joystick.setZAxis(map(hardware.getClutch(), 0, 1023, calibration.values.minClutch, calibration.values.maxClutch));

  int buttonStates[16];
  hardware.getButtonStates(buttonStates);
  int shifterPosition[2];
  hardware.getShifterPosition(shifterPosition);
  int gear = getCurrentGear(shifterPosition, buttonStates);

  for (byte i = 0; i < 7; ++i)
  {
    joystick.setButton(i, LOW);
  }

  if (gear > 0) {
    joystick.setButton(gear - 1, HIGH);
  }

  for (byte i = BUTTON_RED_CENTERRIGHT; i <= BUTTON_DPAD_TOP; ++i) {
    joystick.setButton(buttonTable[i], buttonStates[i]);
  }

  joystick.sendState();
}

void Controller::setup()
{
  Serial.begin(BAUD_RATE);
  attachCommandCallbacks();
  hardware.setup();
  calibration.readFromMemory();
}

void Controller::loop()
{
  // run any commands in queue
  cmdMessenger.feedinSerialData();
  updateJoystick();
}
