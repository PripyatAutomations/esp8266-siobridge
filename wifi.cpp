#include "siobridge.h"
ESP8266WiFiMulti wifiMulti;

const byte DNS_PORT = 53;
IPAddress ap_ip(192, 168, 4, 1);
IPAddress ap_netmask(255, 255, 255, 0);

DNSServer dnsServer;

void dns_setup(void) {
   /* Setup the DNS server redirecting all the domains to the apIP */
   dnsServer.setErrorReplyCode(DNSReplyCode::NoError);

   if (cfg.wifi_mode == WIFI_AP)
      dnsServer.start(DNS_PORT, "*", ap_ip);
   else
      dnsServer.start(DNS_PORT, "*", WiFi.localIP()); 
}

void dns_loop(void) {
   dnsServer.processNextRequest();
}

void dns_stop(void) {
   dnsServer.stop();
}

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
#if	defined(USE_MDNS)
      MDNS.begin(host);
      MDNS.addService("telnet", "tcp", 23);
      MDNS.addService("http", "tcp", 80);
#endif
   } else {
      Serial.printf("Connection status: %d\r\n", WiFi.status());
#if	defined(USE_MDNS)
      MDNS.end();
#endif
   }
}

/* add an access point to our saved configuration
 * XXX: Add to an internal list for config_save() to someday use
 */
void wifi_add_ap(const char *ssid, const char *pass) {
   wifiMulti.addAP(ssid, pass);
}

/* if we do not have a configuration, try to startup a captive portal for config */
void wifi_failsafe(int try_config) {
   bool res = false;

   if (try_config) {
      if (cfg.wifi_ap_ssid != NULL) {
         res = WiFi.softAP(cfg.wifi_ap_ssid, (cfg.wifi_ap_pass != NULL ? cfg.wifi_ap_pass : ""), cfg.wifi_ap_chan, cfg.wifi_ap_hidden, 8);
      }
   } else {
      res = WiFi.softAP(AP_SSID, AP_PASS, AP_CHAN, AP_HIDDEN, 8);
   }

   if (!res) {
      Serial.printf("*** Could not start Soft AP, trying last-ditch config without password...\r\n");

      if (!(res = WiFi.softAP(AP_SSID, ""))) {
         Serial.printf("Nope. Guess we're a brick :(\r\n");
      }
   }
}

void wifi_stop(void) {
   http_stop();
   telnet_stop();
}

/** Is this an IP? */
bool is_ip(String str) {
   for (size_t i = 0; i < str.length(); i++) {
      int c = str.charAt(i);

      if (c != '.' && (c < '0' || c > '9')) {
         return false;
      }
   }
   return true;
}

/** IP to String? */
String ip_to_string(IPAddress ip) {
   String res = "";
   for (int i = 0; i < 3; i++) {
      res += String((ip >> (8 * i)) & 0xFF) + ".";
   }
   res += String(((ip >> 8 * 3)) & 0xFF);
   return res;
}

#if	defined(USE_CAPTIVE_PORTAL)
/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
bool captivePortal() {
   if (!is_ip(httpServer.hostHeader()) && httpServer.hostHeader() != (String(cfg.hostname) + ".local")) {
      Serial.print("* Request redirected to captive portal\r\n");
      httpServer.sendHeader("Location", String("http://") + ip_to_string(server.client().localIP()), true);
      httpserver.send(302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
      httpServer.client().stop();              // Stop is needed because we sent no content length
      return true;
   }
   return false;
}
#endif
