/*
  G27PedalsShifter.cpp

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

#include "G27PedalsShifter.h"

#if defined(_USING_HID)

#define G_REPORT_ID 0x03
#define G_STATE_SIZE 10

static const uint8_t _hidReportDescriptor[] PROGMEM = {
	// Joystick
	0x05, 0x01,			      // USAGE_PAGE (Generic Desktop)
	0x09, 0x04,			      // USAGE (Joystick)
	0xa1, 0x01,			      // COLLECTION (Application)
	0x85, G_REPORT_ID,                  //   REPORT_ID (3)

	// 32 Buttons
	0x05, 0x09,			      //   USAGE_PAGE (Button)
	0x19, 0x01,			      //   USAGE_MINIMUM (Button 1)
	0x29, 0x20,			      //   USAGE_MAXIMUM (Button 32)
	0x15, 0x00,			      //   LOGICAL_MINIMUM (0)
	0x25, 0x01,			      //   LOGICAL_MAXIMUM (1)
	0x75, 0x01,			      //   REPORT_SIZE (1)
	0x95, 0x20,			      //   REPORT_COUNT (32)
	0x55, 0x00,			      //   UNIT_EXPONENT (0)
	0x65, 0x00,			      //   UNIT (None)
	0x81, 0x02,			      //   INPUT (Data,Var,Abs)

	// X, Y, and Z Axis
	0x05, 0x01,			      //   USAGE_PAGE (Generic Desktop)
	0x15, 0x00,			      //   LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x03,       	      //   LOGICAL_MAXIMUM (1023)
	0x75, 0x10,			      //   REPORT_SIZE (16)
	0x09, 0x01,			      //   USAGE (Pointer)
	0xA1, 0x00,			      //   COLLECTION (Physical)
	0x09, 0x30,		              //     USAGE (x)
	0x09, 0x31,		              //     USAGE (y)
	0x09, 0x32,		              //     USAGE (z)
	0x95, 0x03,		              //     REPORT_COUNT (3)
	0x81, 0x02,		              //     INPUT (Data,Var,Abs)
	0xc0,				      //   END_COLLECTION

	0xc0				      // END_COLLECTION
};

G_::G_()
{
	// Setup HID report structure
	static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
	HID().AppendDescriptor(&node);

	// Initalize State
	xAxis = 0;
	yAxis = 0;
	zAxis = 0;
	buttons = 0;
}

void G_::begin(bool initAutoSendState)
{
	autoSendState = initAutoSendState;
	sendState();
}

void G_::end()
{
}

void G_::setButton(uint8_t button, uint8_t value)
{
	if (value == 0)
	{
		releaseButton(button);
	}
	else
	{
		pressButton(button);
	}
}
void G_::pressButton(uint8_t button)
{
	bitSet(buttons, button);
	if (autoSendState) sendState();
}
void G_::releaseButton(uint8_t button)
{
	bitClear(buttons, button);
	if (autoSendState) sendState();
}

void G_::setXAxis(uint16_t value)
{
	xAxis = value;
	if (autoSendState) sendState();
}
void G_::setYAxis(uint16_t value)
{
	yAxis = value;
	if (autoSendState) sendState();
}
void G_::setZAxis(uint16_t value)
{
	zAxis = value;
	if (autoSendState) sendState();
}

void G_::sendState()
{
	uint8_t data[G_STATE_SIZE];
	uint32_t tmp = buttons;

	// Split 32 bit button-state into 4 bytes
	data[0] = tmp & 0xFF;
	tmp >>= 8;
	data[1] = tmp & 0xFF;
	tmp >>= 8;
	data[2] = tmp & 0xFF;
	tmp >>= 8;
	data[3] = tmp & 0xFF;

        // axis get 2 bytes each
        tmp = xAxis;
        data[4] = tmp & 0xFF;
        tmp >>=8;
        data[5] = tmp & 0xFF;

        tmp = yAxis;
        data[6] = tmp & 0xFF;
        tmp >>=8;
        data[7] = tmp & 0xFF;

        tmp = zAxis;
        data[8] = tmp & 0xFF;
        tmp >>=8;
        data[9] = tmp & 0xFF;

	// HID().SendReport(Report number, array of values in same order as HID descriptor, length)
	HID().SendReport(G_REPORT_ID, data, G_STATE_SIZE);
}

G_ G;

#endif
