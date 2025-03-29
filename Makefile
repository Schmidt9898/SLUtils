ARDUINO_CLI=arduino-cli
BOARD=arduino:avr:nano
#BOARD=arduino:avr:mega
BUILD_PATH=./_build/
LIBRARY_PATH=./
SKETCH_PATH=./tests/buildtest/
#PORT=COM6
PORT=COM4

build:
	$(ARDUINO_CLI) compile $(SKETCH_PATH) -b $(BOARD) --build-path $(BUILD_PATH) --library $(LIBRARY_PATH)


MCU=m328p
#MCU=m2560
BAUD=115200
#BAUD=57600
HEX_FILE=$(BUILD_PATH)/buildtest.ino.hex
EEP_FILE=$(BUILD_PATH)/buildtest.ino.eep

upload:
#	avrdude -p $(MCU) -c arduino -P $(PORT) -b $(BAUD) -U flash:w:$(HEX_FILE):i
#	avrdude -p $(MCU) -c arduino -P $(PORT) -b $(BAUD) -U eeprom:w:$(EEP_FILE):i
#	avrdude -p $(MCU) -c arduino -P $(PORT) -b $(BAUD) -D -U flash:w:$(HEX_FILE):i -U eeprom:w:$(EEP_FILE):i
	arduino-cli upload --build-path $(BUILD_PATH) -b $(BOARD) -p $(PORT)

#upload:

check:
	avrdude -p $(MCU) -c arduino -P $(PORT) -b $(BAUD) -U flash:r:flash_dump.hex:i
#	avrdude -p $(MCU) -c arduino -P $(PORT) -b $(BAUD) -U eeprom:r:eeprom_dump.hex:i
#	avrdude -p $(MCU) -c arduino -P $(PORT) -b $(BAUD) -U eeprom:r:eeprom_dump.bin:r



clean:
	rm -rf $(BUILD_PATH)

.PHONY: compile clean




