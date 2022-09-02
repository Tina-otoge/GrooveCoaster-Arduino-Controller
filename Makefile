SKETCH:=GC_Native
CORE=arduino:avr
BOARD=leonardo
TTY=/dev/ttyACM0

all: install compile flash

install: install-deps
	arduino-cli core update-index
	arduino-cli core install $(CORE)

compile:
	arduino-cli compile --fqbn $(CORE):$(BOARD) ./$(SKETCH)

flash:
	arduino-cli upload -p $(TTY) --fqbn $(CORE):$(BOARD) ./$(SKETCH)

install-deps:
	mkdir -p $(HOME)/Arduino/libraries
	cd $(HOME)/Arduino/libraries && [ -d SwitchControlLibrary ] || git clone https://github.com/celclow/SwitchControlLibrary
	cd $(HOME)/Arduino/libraries && [ -d ArduinoSTL ] || git clone https://github.com/mike-matera/ArduinoSTL

monitor:
	arduino-cli monitor -p $(TTY) --fqbn $(CORE):$(BOARD)

.PHONY: all install compile flash install-deps monitor
