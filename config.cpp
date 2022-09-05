#include <Arduino.h>
#include <ESP8266WiFi.h> 
#include <Hash.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiManager.h>
#include <FS.h>
#include <string.h>

bool config_load(void) {
   Serial.println("* Loading config.txt");
   File cf = SPIFFS.open("/config.txt", "r"); 

   if (!cf) {
      Serial.print("ERROR Opening config.txt failed");
      return false;
   }

   /* Parse the configuration */
}

