#if	!defined(_SIOBRIDGE_H)
#define	_SIOBRIDGE_H

#define	VERSION "20220911.01"
#include <Arduino.h>
#include <ESP8266WiFi.h> 
#include <Hash.h>
#include <DNSServer.h>
#include <ESP8266WiFiMulti.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <string.h>
#include <Hash.h>
#include <functional>
#include "config.h"
#include "permissions.h"
#include "users.h"
#include "ansi.h"
#include "console.h"
#include "crc.h"
#include "fs.h"
#include "forwarding.h"
#include "logger.h"
#include "telnet.h"
#include "mdns.h"
#include "ntp.h"
#include "ota.h"
#include "sessions.h"
#include "sha1.h"
#include "telnet.h"
#include "utility.h"
#include "wifi.h"

extern bool need_reboot;

#endif
