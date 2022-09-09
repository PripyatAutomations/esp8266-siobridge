#include "siobridge.h"
WiFiManager wifiManager;
ESP8266WiFiMulti wifiMulti;

WiFiEventHandler wifi_disconnected = WiFi.onStationModeDisconnected([]( const WiFiEventStationModeDisconnected& event) {
   Serial.println("Station disconnected");
   telnet_stop();
   http_stop();
}); 

WiFiEventHandler wifi_ip_active =  WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event) {
   Serial.print("Station connected, IP: ");
   Serial.println(WiFi.localIP());
   telnet_setup();
   http_setup();
});

/* Initialize wifi configurations */
void wifi_setup(void) {
   /* default wifi to station mode */
   WiFi.mode(WIFI_STA);
   WiFi.setSleepMode(WIFI_NONE_SLEEP);

   if (cfg.wifi_power > 0)
      WiFi.setOutputPower(cfg.wifi_power);
}

/* called from main_loop() to service network clients */
void wifi_loop(void) {
   if (wifiMulti.run(cfg.wifi_timeout) == WL_CONNECTED) {
      telnet_loop();
      http_loop();
/*
      MDNS.begin(host);
      MDNS.addService("telnet", "tcp", 23);
      MDNS.addService("http", "tcp", 80);
*/
   } else {
      Serial.printf("Connection status: %d\n", WiFi.status());
/*
      MDNS.end();
*/
   }
}

/* add an access point to our saved configuration
 * XXX: Add to an internal list for config_save() to someday use
 */
void wifi_add_ap(const char *ssid, const char *pass) {
   wifiMulti.addAP(ssid, pass);
}

/* if we do not have a configuration, try to startup a captive portal for config */
void wifi_failsafe(void) {
   wifiManager.setConfigPortalTimeout(AP_CONFIG_TIMEOUT);
   wifiManager.autoConnect(AP_SSID);
}