#ifndef Controller_h
#define Controller_h

#include "Globals.h"
#include "CmdMessenger.h"
#include "HardwareHelper.h"
#include "CalibrationHelper.h"
#include "Joystick.h"

enum Command
{
  kAcknowledge,
  kError,
  kGetStatus,
  kGetStatusResult,
  kGetCalibration,
  kGetCalibrationResult,
  kSetCalibration,
  kSetCalibrationResult
};

class Controller
{
protected:
  static HardwareHelper hardware;
  static CmdMessenger cmdMessenger;
  static CalibrationHelper calibration;
  static Joystick joystick;

  void attachCommandCallbacks();
  static void onUnkownCommand();
  static void onAcknowledge();
  static void onGetStatus();
  static void onGetCalibration();
  static void onSetCalibration();
  static void updateJoystick();
  static int getCurrentGear(int *shifterPosition, int *buttonStates);

public:
  static Controller instance;
  void setup();
  void loop();
};

#endif
