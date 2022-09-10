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
#include <EspSyslog/Syslog.h>
#include <FS.h>
#include <string.h>
#include <Hash.h>
#include <functional>
#include "config.h"
#include "users.h"
#define	STRINGIFY(x)	#x

/* main.c */
extern bool need_reboot;

/* syslog */
extern Syslog *logger;
extern void syslog_setup(void);

/* wifi stuff */
extern ESP8266WiFiMulti wifiMulti;
extern void wifi_setup(void), wifi_loop(void), wifi_stop(void);
extern void wifi_add_ap(const char *ssid, const char *pass);
extern void wifi_failsafe(int try_config);
extern boolean is_ip(String str);
extern String ip_to_string(IPAddress ip);
extern bool captivePortal();

/* telnet/http services */
extern void telnet_loop(void), http_loop(void);
extern void telnet_setup(void), http_setup(void);
extern void telnet_stop(void), http_stop(void);

/* serial relay service */
extern void relay_setup(void), relay_loop(void);

/* file system */
extern bool spiffsActive;
#if	defined(USE_SPIFFS)
extern SPIFFSConfig FScfg;
#endif
#if	defined(USE_LITTLEFS)
extenr LittleFSConfig FScfg;
#endif
extern Dir fs_root, fs_rnode;
extern void flash_init(void);
extern void flash_gc(void);
extern void flash_dir(void);

/* console */
extern const char *console_prompt(Stream *ch, const char *prompt);
extern const char *redact_password(const char *p);
extern bool show_menu(Stream *ch, const char *menu);

/* mdns service */
extern void mdns_setup(void), mdns_stop(void), mdns_loop(void);

#endif
