# G27 Pedals and Shifter

Arduino-based USB interface for Logitech G27 pedals and shifter:

![on breadboard](https://raw.githubusercontent.com/functionreturnfunction/G27_Pedals_and_Shifter/master/Breadboard.jpg)
![in altoids tin](https://raw.githubusercontent.com/functionreturnfunction/G27_Pedals_and_Shifter/master/Altoids Tin.jpg)

## Required Parts/Materials

* [SparkFun Pro Micro](https://www.sparkfun.com/products/12640) or clone (must be a 5V/16MHz ATmega32U4 with onboard USB)
* [DB9 Connectors](http://www.amazon.com/Female-Male-Solder-Adapter-Connectors/dp/B008MU0OR4/ref=sr_1_1?ie=UTF8&qid=1457291922&sr=8-1&keywords=db9+connectors) 1 male, 1 female
* Hookup wire in assorted colors (I used red, black, blue, green, purple, yellow, orange, and white)
* Some kind of project box (I used an Altoids tin)

## Assembly

Connect the female DB9 connector for the pedals to the board using the pins in the table in the .ino file.  Do the same with the male DB9 for the shifter.

NOTE: when wiring the male connector for the shifter, remember that the pins will read right to left rather than left to right.

## Firmware

Open the .ino file in the Arduino IDE, select the proper board type and COM port under "Tools" (you will need to install the [SparkFun board library](https://github.com/sparkfun/Arduino_Boards)).  You will probably need to adjust the thresholds for SHIFTER_XAXIS_12 and friends, the values that decide which gear you're in based on the x/y axis of the shifter.  Uncomment the `#define DEBUG_SHIFTER true` line near the top to get debugging information to aid in this process.

## Calibration and Configration

The pedals are self-calibrating, meaning the system determines the min/max value for each pedal in realtime.  What this means is that each time the device is powered on, you'll need to push each of the three pedals all the way to the floor once to let it know what the maximums are.

When configuring the shifter and buttons, gears 1 - 6 are buttons 1 - 6, reverse is button 7, and then the rest of the buttons enumerate from there starting from the top black button.
