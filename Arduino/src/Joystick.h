/*
  Joystick.h

  Copyright (c) 2016, Jason Duncan

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

// (stolen from Matthew Heironimus @ https://github.com/MHeironimus/ArduinoJoystickLibrary)

#ifndef Joystick_h
#define Joystick_h

#include "HID.h"

#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable)"

#else

//================================================================================
//================================================================================
//  G27 (Gamepad)

class Joystick
{
private:
	bool     autoSendState;
	uint16_t xAxis;
	uint16_t yAxis;
	uint16_t zAxis;
	uint32_t buttons;

public:
	Joystick();

	void begin(bool initAutoSendState = true);
	void end();

	void setXAxis(uint16_t value);
	void setYAxis(uint16_t value);
	void setZAxis(uint16_t value);

	void setButton(uint8_t button, uint8_t value);
	void pressButton(uint8_t button);
	void releaseButton(uint8_t button);

	void sendState();
};

#endif
#endif
