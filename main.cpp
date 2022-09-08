/* TODO: Make this fully async */
#include "siobridge.h"
bool spiffsActive = false;
int RESET_PIN = 0; // = GPIO0 on nodeMCU
Dir fs_root, fs_rnode;
SPIFFSConfig SPIFFScfg;
ESP8266WiFiMulti wifiMulti;
extern void telnet_loop(void);
extern void telnet_setup(void);

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

   /* default wifi to station mode */
   WiFi.mode(WIFI_STA);
   WiFi.setSleepMode(WIFI_NONE_SLEEP);

   /* Setup filesystem */
   SPIFFScfg.setAutoFormat(false);
   SPIFFS.setConfig(SPIFFScfg);

   if (SPIFFS.begin()) {
      Serial.println("* FS active!");
      spiffsActive = true;
   } else {
      Serial.println("* FS corrupt!");
   }
   fs_root = fs_rnode = SPIFFS.openDir("/");

   config_load();

//   Serial.println("* Activating WiFi");

   telnet_setup();
   http_setup();

}

void main_loop(void) {
  if (wifiMulti.run(cfg.wifi_timeout) == WL_CONNECTED) {
     telnet_loop();
     http_loop();
  }
}
