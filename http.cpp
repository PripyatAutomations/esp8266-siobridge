#include "siobridge.h"
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void http_setup(void) {
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
   while(fs_rnode.next()) {
      File file = fs_rnode.openFile("r");

      /* Skip some files that shouldn't be served to users */
      if (strncasecmp(file.name(), "/config.txt", 11) == 0 ||
          strncasecmp(file.name(), "/passwd", 6) == 0) {
#if	defined(SIO_DEBUG)
         Serial.printf("* skip %s (system config)\r\n", file.name());
#endif
         continue;
      }

#if	defined(SIO_DEBUG)
      Serial.printf("* + %s\r\n", file.name());
#endif
      httpServer.serveStatic(file.name(), SPIFFS, file.name());
   }

   /* Enable firmware updater at /firmware */
   httpUpdater.setup(&httpServer, "/firmware", cfg.admin_user, cfg.admin_pass);
   httpServer.on("/status", []() {
      Serial.println("* Serving status page to http client");
      httpServer.send(200, "text/html", "Status isn't ready yet...\n");
   });
   httpServer.on("/wifi-config", []() {
      String content = "Please connect to ";
             content += AP_SSID;
             content += " AP and configure then <a href=\"/\">Click Here</a> to open configuration page.\n";
      Serial.println("*** Switching to AP mode for configuration by admin request ***");

      httpServer.send(200, "text/html", content);
      /* Switch to AP mode */
   });
   httpServer.begin();
}

void http_loop(void) {
   httpServer.handleClient();
}

void http_stop(void) {
}

