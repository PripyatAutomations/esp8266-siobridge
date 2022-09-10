What it is?
-----------
esp8266-siobridge is a simple serial to telnet and http bridge for your
project.

You connect a few GPIO to your other device(s) serial port(s) and edit
data/config.txt to contain a power on configuration.

Then run 'make world upload upload_fs' to build and install on your
connected esp8266.


Needed Libraries
----------------
* esp8266-core
* ESPAsyncTCP
* ESPAsyncWebserver


Looking around the code, you'll see a lot of stubs for things that will be
helpful for others, but aren't a priority at all for me right now.

Feel free to implement additional features and send a pull request or .diff!

Right now i'm focusing on the following things:
* console
* telnet to serial ports
* OTA update
* File System


Long term goals:
* Status pages
* Multiple serial ports (tx/rx only, etc)
* bridge2bridge mode
* Support for non-8N1 ports
* Support for i2c/1-wire/etc via arduino's Streams wrappers.
