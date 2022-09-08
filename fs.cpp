#include "siobridge.h"
bool spiffsActive = false;
Dir fs_root, fs_rnode;
SPIFFSConfig SPIFFScfg;

void flash_init() {
   uint32_t realSize = ESP.getFlashChipRealSize();
   uint32_t ideSize = ESP.getFlashChipSize();
   FlashMode_t ideMode = ESP.getFlashChipMode();
   /* Verify flash configuration is valid */
   Serial.printf("* Flash real id:   %08X\r\n", ESP.getFlashChipId());
   Serial.printf("* Flash real size: %u bytes\r\n", realSize);
   Serial.printf("* Flash ide  size: %u bytes\r\n", ideSize);
   Serial.printf("* Flash ide speed: %u Hz\r\n", ESP.getFlashChipSpeed());
   Serial.printf("* Flash ide mode:  %s\r\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO  ? "DIO" : ideMode == FM_DOUT ? "DOUT"  : "UNKNOWN"));

   if (ideSize != realSize) {
      Serial.println("* Flash cfg invalid!");
   } else {
      Serial.println("* Flash cfg ok.");
   }
   /* Setup filesystem */
   SPIFFScfg.setAutoFormat(false);
   SPIFFS.setConfig(SPIFFScfg);

   if (SPIFFS.begin()) {
      Serial.println("* FS active!");
      spiffsActive = true;
   } else {
      Serial.println("* FS corrupt, trying to repair!");
      SPIFFS.check();
      
   }
   fs_root = fs_rnode = SPIFFS.openDir("/");
}

void flash_gc(void) {
   SPIFFS.gc();
}

void flash_dir(void) {
   Dir dir = SPIFFS.openDir("/");
   String str = "";

   while (dir.next()) {
      str += "* flash:";
      str += dir.fileName();
      str += " is ";
      str += dir.fileSize();
      str += " bytes\r\n";
   }
   Serial.print(str);
}
