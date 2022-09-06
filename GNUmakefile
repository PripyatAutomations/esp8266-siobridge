all: world
CHIP=esp8266
BOARD=d1_mini_clone
FS_TYPE=spiffs
FS_DIR=data/
FLASH_DEF=4M2M
VERBOSE=1
MONITOR_SPEED=115200

include *.mk
world:

clean:

install-fw: upload upload-fs

connect:
	picocom --baud 115200 /dev/ttyUSB0 
