#define	VERSION "20220905.01"
#include <Arduino.h>
#include <ESP8266WiFi.h> 
#include <Hash.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WiFiMulti.h>
#include <FS.h>
#include <string.h>
#include "config.h"

#define	SIO_DEBUG

/* Wait up to 300 seconds (5 minutes) in the WiFi config (AP) mode before restarting */
#define AP_CONFIG_TIMEOUT 300
#define AP_SSID   "REPEATER ADMIN"
#define ADMIN_USER "admin"
#define ADMIN_PASS "admin"

extern bool config_load(void);
extern ESP8266WiFiMulti wifiMulti;
extern void http_setup(void);
extern void http_loop(void);
extern Dir fs_root, fs_rnode;
extern ESP8266WiFiMulti wifiMulti;

