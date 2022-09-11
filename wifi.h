#if	!defined(_SIOBRIDGE_wifi_h)
#define	_SIOBRIDGE_wifi_h

/* wifi stuff */
extern ESP8266WiFiMulti wifiMulti;
extern void wifi_setup(void), wifi_loop(void), wifi_stop(void);
extern void wifi_add_ap(const char *ssid, const char *pass);
extern void wifi_failsafe(int try_config);
extern boolean is_ip(String str);
extern String ip_to_string(IPAddress ip);
extern bool captivePortal();

#endif
