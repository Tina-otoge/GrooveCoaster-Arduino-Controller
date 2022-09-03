Fork of Meta-Link's Arduino firmware for use with a Groove Coaster controller as
a Nintendo Switch controller.

# GrooveCoaster Arduino Controller

## How to tweak / flash / edit

Makefile for compiling and flashing using `arduino-cli`, only tested on Linux.

You will need `arduino-cli`, which you cat get from there: https://arduino.github.io/arduino-cli/latest/installation/#download

You will need to update your board's build PID/VID. Head to `~/.arduinoXX/packages/arduino/hardware/avr/X.X.X/boards.txt`,
search for your board (ie: `leonardo`) and change `yourboard.build.vid=` to
`0x0f0d` and `yourboard.build.pid` to `0x0092`. This is necessary to spoof the
Switch Controller Pro.

Run `make` to download the dependencies, build and flash the firmware to your board.


The reset button of the Leonardo is right under the micro-USB port.

The `config.h` file contains a bunch of options you can tweak.


By enabling serial log output, the controller will output the state of every
button each second.


The "Debug" sketch is used to only report the state of each pins as soon as they
are pressed, useful to find which button is bound to which pin. The pins 12 and
13 are skipped because I did not use them, comment the `if` block if you use
them.


## Hopefully temporary dirty fixes

Due to bugs in newer versions of  SwitchControlLibrary, I backported some
changes to the v1.0.0 and use this one.


As of 2022-09-03, mike-matera/AndroidSTL has a bug that blocks compilation due
to "nothrow" now existing in the embedded C++ implementation, I used this branch
in the meantime https://github.com/mike-matera/ArduinoSTL/pull/82.
