#include "siobridge.h"
bool spiffsActive = false;
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
const char *update_username = ADMIN_USER;
const char *update_password = ADMIN_PASS;

//#define MAX_SRV_CLIENTS 1
WiFiServer server(23);
WiFiClient serverClient;

int RESET_PIN = 0; // = GPIO0 on nodeMCU
WiFiManager wifiManager;
Dir root, rnode;

void flash_init() {
   uint32_t realSize = ESP.getFlashChipRealSize();
   uint32_t ideSize = ESP.getFlashChipSize();
   FlashMode_t ideMode = ESP.getFlashChipMode();
   /* Verify flash configuration is valid */
   Serial.printf("* Flash real id:   %08X\r\n", ESP.getFlashChipId());
   Serial.printf("* Flash real size: %u bytes\r\n", realSize);
   Serial.printf("* Flash ide  size: %u bytes\r\n", ideSize);
   Serial.printf("* Flash ide speed: %u Hz\r\n", ESP.getFlashChipSpeed());
   Serial.printf("* Flash ide mode:  %s\r\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT"
                                                                     : ideMode == FM_DIO  ? "DIO"
                                                                     : ideMode == FM_DOUT ? "DOUT"
                                                                                          : "UNKNOWN"));

   if (ideSize != realSize) {
      Serial.println("* Flash cfg invalid!");
   } else {
      Serial.println("* Flash cfg ok.");
   }
}

void main_setup(void) {
   /* enable reset pin */
   pinMode(RESET_PIN, INPUT_PULLUP);

   /* Setup serial console */
   Serial.setRxBufferSize(1024);
   Serial.begin(115200);

   Serial.print("\r\n\r\n\r\n\r\n\r\nBOOTWAIT ");
   for (int i = 5; i > 0; i--) {
       Serial.printf("%d... ", i);
       delay(1000);
   }
   Serial.print("\r\n\r\n");
   Serial.printf("esp8266-siobridge %s starting\n", VERSION);
   flash_init();

   if (SPIFFS.begin()) {
      Serial.println("* FS active!");
      spiffsActive = true;
   } else {
      Serial.println("* FS corrupt!");
   }

   root = rnode = SPIFFS.openDir("/");
   config_load();

   Serial.println("* Activating WiFi");
   /* try to connect to saved wifi, if not give AP for config */
   wifiManager.setConfigPortalTimeout(AP_CONFIG_TIMEOUT);
   wifiManager.autoConnect(AP_SSID);

   Serial.println("* Building http root...");

   /*************
    * Setup www *
    *************/
   httpServer.onNotFound([]() {
      httpServer.send(404, "text/html", "That page couldn't found...<br><a href=\"/\">Click Here</a> to go back<br/>\n");
   });
   /* Point / at index.html */
   httpServer.serveStatic("/", SPIFFS, "/index.html");

   /* add static files */
   while(rnode.next()) {
      File file = rnode.openFile("r");

      /* Skip some files that shouldn't be served to users */
      if (strncasecmp(file.name(), "/config.txt", 11) == 0 ||
          strncasecmp(file.name(), "/passwd", 6) == 0) {
#if	defined(DEBUG)
         Serial.printf("* skip %s (system config)\r\n", file.name());
#endif
         continue;
      }

#if	defined(DEBUG)
      Serial.printf("* + %s\r\n", file.name());
#endif
      httpServer.serveStatic(file.name(), SPIFFS, file.name());
   }

   /* start the telnet server */
   server.begin();
   server.setNoDelay(true);

   /* Enable firmware updater at /firmware */
   httpUpdater.setup(&httpServer, "/firmware", update_username, update_password);
   httpServer.on("/status", []() {
      Serial.println("* Serving status page to http client");
      httpServer.send(200, "text/html", "Status isn't ready yet...\n");
   });
   httpServer.on("/wifi", []() {
      String content = "Please connect to ";
             content += AP_SSID;
             content += " AP and configure then <a href=\"/\">Click Here</a> to open configuration page.\n";
      Serial.println("*** Switching to AP mode for configuration by admin request ***");

      httpServer.send(200, "text/html", content);

      if (!wifiManager.startConfigPortal(AP_SSID)) {
         Serial.println("failed to connect and hit timeout");
         delay(3000);
         ESP.restart();
         delay(5000);
      }
   });
   httpServer.begin();

   WiFi.setSleepMode(WIFI_NONE_SLEEP);
}


void AcceptConnection(void) {
   if (serverClient && serverClient.connected()) 
      serverClient.stop();

   serverClient = server.available();
   serverClient.write("esp8266-siobridge ");
   serverClient.write(VERSION);
   serverClient.write(" ready!\n");
}

void ManageConnected(void) {
   size_t rxlen = serverClient.available();

   if (rxlen > 0) {
      uint8_t sbuf[rxlen];
      serverClient.readBytes(sbuf, rxlen);
      Serial.write(sbuf, rxlen);
    }
	
    size_t txlen = Serial.available();

    if (txlen > 0) {
       uint8_t sbuf[txlen];
       Serial.readBytes(sbuf, txlen);
       serverClient.write(sbuf, txlen);
    }
}

void main_loop(void) {
   /* Accept new pending telnet clients */
   if (server.hasClient())
      AcceptConnection();
   else if (serverClient && serverClient.connected()) {
      /* XXX: Add multi-user support */
      /* Handle already connected telnet users */
      ManageConnected();
   }

   /* Handle http requests */
   httpServer.handleClient();
}
