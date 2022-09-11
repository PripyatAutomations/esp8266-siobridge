#if	!defined(_SIOBRIDGE_H)
#define	_SIOBRIDGE_H

#define	VERSION "20220908.01"
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
#include "users.h"

extern bool need_reboot;

#include "logger.h"
#include "wifi.h"
#include "telnet.h"
#include "fs.h"
#include "mdns.h"
#include "sessions.h"
#include "forwarding.h"
#include "console.h"
#include "utility.h"
#include "ntp.h"

#endif
