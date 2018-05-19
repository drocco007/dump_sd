# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

BOARD_TAG    = uno
MONITOR_PORT = /dev/ttyACM0
ARDUINO_LIBS = SPI SD

include ${ARDMK_DIR}/Arduino.mk
