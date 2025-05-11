
ARDUINO_CLI=arduino-cli
BOARD=arduino:avr:nano
#BOARD=arduino:avr:mega
BUILD_PATH=./_build/
LIBRARY_PATH=../../
SKETCH_PATH=./
#PORT=COM6
PORT=COM4

build:
	$(ARDUINO_CLI) compile $(SKETCH_PATH) -b $(BOARD) --build-path $(BUILD_PATH) --library $(LIBRARY_PATH)


upload:
	arduino-cli upload --build-path $(BUILD_PATH) -b $(BOARD) -p $(PORT)