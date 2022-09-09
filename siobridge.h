#if	!defined(_SIOBRIDGE_H)
#define	_SIOBRIDGE_H

#define	VERSION "20220908.01"
#include <Arduino.h>
#include <ESP8266WiFi.h> 
#include <Hash.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiManager.h>
#include <FS.h>
#include <string.h>
#include "config.h"

#define	SIO_DEBUG	1

/* Use either SPIFFS or LittleFS, not both! */
#define	USE_SPIFFS
//#define USE_LITTLEFS

/* Wait up to 300 seconds (5 minutes) in the WiFi config (AP) mode before restarting */
#define AP_CONFIG_TIMEOUT 300
#define AP_SSID   "REPEATER ADMIN"
#define	AP_PASS	  ""
#define ADMIN_USER "admin"
#define ADMIN_PASS "admin"

/* wifi stuff */
extern ESP8266WiFiMulti wifiMulti;
extern void wifi_setup(void), wifi_loop(void);
extern void wifi_add_ap(const char *ssid, const char *pass);
extern void wifi_failsafe(int try_config);

/* telnet/http services */
extern void telnet_loop(void), http_loop(void);
extern void telnet_setup(void), http_setup(void);
extern void telnet_stop(void), http_stop(void);

/* serial relay service */
extern void relay_setup(void), relay_loop(void);

/* file system */
extern bool spiffsActive;
extern SPIFFSConfig SPIFFScfg;
extern Dir fs_root, fs_rnode;
extern void flash_init(void);
extern void flash_gc(void);
extern void flash_dir(void);
#endif
