CHIP=esp8266
BOARD=d1_mini_clone
FS_TYPE=spiffs
FS_DIR=data/
FLASH_DEF=4M2M
VERBOSE=1
MONITOR_SPEED=115200
HTTP_USR=admin
HTTP_PWD=password
OTA_PWD=admin

LIBS += ESPAsyncTCP ESPAsyncWebServer esp8266_mdns EspSyslog ArduinoOTA GDBStub

world: all

include config.mk
include makeEspArduino.mk

all: host-all espduino-all

host-all:
	${MAKE} -C host all

host-install:
	${MAKE} -C host install

host-clean:
	${MAKE} -C host clean

clean: espduino-clean host-clean
install-fw: upload upload-fs

connect:
	picocom --baud 115200 /dev/ttyUSB0 
