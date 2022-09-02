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
