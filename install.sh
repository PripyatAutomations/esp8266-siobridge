#!/bin/sh
[ ! -f ~/Arduinp/tools/ESP8266FS/tool/esp8266fs.jar ] && {
	echo "* Installing Arduino ESP8266FS uploader into arduino stupid for $(whoami)..."
	mkdir -p ~/Arduino/tools
	cp -r ESP8266FS ~/Arduino/tools
}
