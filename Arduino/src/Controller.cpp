#include "Controller.h"

CmdMessenger Controller::cmdMessenger = CmdMessenger(Serial);
HardwareHelper Controller::hardware;
CalibrationHelper Controller::calibration;

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

  cmdMessenger.sendCmdArg(hardware.getGas());
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
}
