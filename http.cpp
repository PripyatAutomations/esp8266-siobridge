#include "siobridge.h"
AsyncWebServer httpServer(cfg.http_port);
AsyncWebSocket ws("/ws");		// access at ws://[esp ip]/ws
AsyncEventSource events("/events"); 	// event source (Server-Sent events)

void onRequest(AsyncWebServerRequest *request){
  //Handle Unknown Request
  request->send(404);
}

void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  //Handle body
}

void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
  //Handle upload
}

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  //Handle WebSocket event
}

void http_add_scan(void) {
   //First request will return 0 results unless you start scan from somewhere else (loop/setup)
   //Do not request more often than 3-5 seconds
   httpServer.on("/scan", HTTP_GET, [](AsyncWebServerRequest *request){
     String json = "[";
     int n = WiFi.scanComplete();
     if(n == -2){
       WiFi.scanNetworks(true);
     } else if(n){
       for (int i = 0; i < n; ++i){
         if(i) json += ",";
         json += "{";
         json += "\"rssi\":"+String(WiFi.RSSI(i));
         json += ",\"ssid\":\""+WiFi.SSID(i)+"\"";
         json += ",\"bssid\":\""+WiFi.BSSIDstr(i)+"\"";
         json += ",\"channel\":"+String(WiFi.channel(i));
         json += ",\"secure\":"+String(WiFi.encryptionType(i));
         json += ",\"hidden\":"+String(WiFi.isHidden(i)?"true":"false");
         json += "}";
       }
       WiFi.scanDelete();
       if(WiFi.scanComplete() == -2){
         WiFi.scanNetworks(true);
       }
     }
     json += "]";
     request->send(200, "application/json", json);
     json = String();
   });
}

void http_setup(void) {
   Serial.println("* Building http root...");

   // attach AsyncWebSocket
   ws.onEvent(onEvent);
   httpServer.addHandler(&ws);

   // attach AsyncEventSource
   httpServer.addHandler(&events);

   // respond to GET requests on URL /heap
   httpServer.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
     request->send(200, "text/plain", String(ESP.getFreeHeap()));
   });

   // upload a file to /upload
   httpServer.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request){
     request->send(200);
   }, onUpload);

   // send a file when /index is requested
   httpServer.on("/index", HTTP_ANY, [](AsyncWebServerRequest *request){
     request->send(SPIFFS, "/index.htm");
   });

   // HTTP basic authentication
   httpServer.on("/login", HTTP_GET, [](AsyncWebServerRequest *request){
     if(!request->authenticate(cfg.admin_user, cfg.admin_pass))
         return request->requestAuthentication();
     request->send(200, "text/plain", "Login Success!");
   });

   // Simple Firmware Update Form
   httpServer.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
     request->send(200, "text/html", "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>");
   });
   httpServer.on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
     need_reboot = !Update.hasError();
     AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", need_reboot ? "OK" : "FAIL");
     response->addHeader("Connection", "close");
     request->send(response);
   },[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
     if(!index){
       Serial.printf("Update Start: %s\n", filename.c_str());
       Update.runAsync(true);
       if(!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)){
         Update.printError(Serial);
       }
     }
     if(!Update.hasError()){
       if(Update.write(data, len) != len){
         Update.printError(Serial);
       }
     }
     if(final){
       if(Update.end(true)){
         Serial.printf("Update Success: %uB\n", index+len);
       } else {
         Update.printError(Serial);
       }
     }
   });

   // attach filesystem root at URL /fs
   httpServer.serveStatic("/fs", SPIFFS, "/");

   // Catch-All Handlers
   // Any request that can not find a Handler that canHandle it
   // ends in the callbacks below.
   httpServer.onNotFound(onRequest);
   httpServer.onFileUpload(onUpload);
   httpServer.onRequestBody(onBody);

#if	0
   /*************
    * Setup www *
    *************/
   httpServer.onNotFound([]() {
      httpServer.send(404, "text/html", "That page couldn't found...<br><a href=\"/\">Click Here</a> to go back<br/>\n");
   });
   /* Point / at index.html */
#if	defined(USE_SPIFFS)
   httpServer.serveStatic("/", SPIFFS, "/index.html");
#endif
#if	defined(USE_LITTLEFS)
   httpServer.serveStatic("/", SPIFFS, "/index.html");
#endif

   /* add static files */
   while(fs_rnode.next()) {
      File file = fs_rnode.openFile("r");

      /* Skip some files that shouldn't be served to users */
      if (strncasecmp(file.name(), "/config.txt", 11) == 0 ||
          strncasecmp(file.name(), "/passwd", 6) == 0) {
         Serial.printf("* skip %s (system config)\r\n", file.name());
         continue;
      }

#if	defined(SIO_DEBUG)
      Serial.printf("* http +%s\r\n", file.name());
#endif
      httpServer.serveStatic(file.name(), SPIFFS, file.name());
   }

   httpServer.on("/wifi-config", []() {
      String content = "Please connect to ";
             content += AP_SSID;
             content += " AP and configure then <a href=\"/\">Click Here</a> to open configuration page.\n";
      Serial.println("*** Switching to AP mode for configuration by admin request ***");

      httpServer.send(200, "text/html", content);

      /* Switch to AP mode */
      wifi_failsafe(true);
      return;
   });
#endif
#if	0
   // OTA callbacks
   ArduinoOTA.onStart([]() {
     // Clean SPIFFS
     SPIFFS.end();

     // Disable client connections    
     ws.enable(false);

     // Advertise connected clients what's going on
     ws.textAll("OTA Update Started");

     // Close them
     ws.closeAll();

   });
#endif
   httpServer.begin();
}

void http_loop(void) {
   static char temp[128];
   sprintf(temp, "Seconds since boot: %u", millis()/1000);
   events.send(temp, "time"); //send event "time"
}

void http_stop(void) {
//   httpServer.stop();
}
