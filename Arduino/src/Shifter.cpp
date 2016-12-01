#include "Shifter.h"

void Shifter::waitForSignalToSettle()
{
  delayMicroseconds(SIGNAL_SETTLE_DELAY);
}

void Shifter::getButtonStates(int *ret)
{
  digitalWrite(SHIFTER_MODE_PIN, LOW);    // Switch to parallel mode: digital inputs are read into shift register
  waitForSignalToSettle();
  digitalWrite(SHIFTER_MODE_PIN, HIGH);   // Switch to serial mode: one data bit is output on each clock falling ed

  for(int i = 0; i < 16; ++i) {           // Iteration over both 8 bit registers
    digitalWrite(SHIFTER_CLOCK_PIN, LOW);         // Generate clock falling edge
    waitForSignalToSettle();

    ret[i] = digitalRead(SHIFTER_DATA_PIN);

    digitalWrite(SHIFTER_CLOCK_PIN, HIGH);        // Generate clock rising edge
    waitForSignalToSettle();
  }
}

void Shifter::getShifterPosition(int *ret)
{
  ret[0] = analogRead(SHIFTER_X_PIN);
  ret[1] = analogRead(SHIFTER_Y_PIN);
}

