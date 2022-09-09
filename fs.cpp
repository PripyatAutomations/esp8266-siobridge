#include "siobridge.h"

bool fs_active = false;
Dir fs_root, fs_rnode;
#if	defined(USE_SPIFFS)
SPIFFSConfig FScfg;
#endif
#if	defined(USE_LITTLEFS)
LittleFSConfig FScfg
#endif

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
      Serial.printf("* Flash cfg invalid!\r\n");
   } else {
      Serial.printf("* Flash cfg ok.\r\n");
   }

   /* Verify flash CRC */
   if (ESP.checkFlashCRC() == false) {
      Serial.printf("\r\n");
   }
   /* Setup filesystem */
   FScfg.setAutoFormat(false);

#if	defined(USE_SPIFFS)
   SPIFFS.setConfig(FScfg);

   if (SPIFFS.begin()) {
#endif
#if	defined(USE_LITTLEFS)
   LittleFS.setConfig(FScfg);

   if (LittleFS.begin()) {
#endif
      Serial.printf("* FS active!\r\n");
      fs_active = true;
   } else {
      Serial.printf("* FS corrupt, trying to repair!\r\n");
#if	defined(USE_SPIFFS)
      SPIFFS.check();
#endif
#if	defined(USE_LITTLEFS)
      LittleFS.check();
#endif
   }
#if	defined(USE_SPIFFS)
   fs_root = fs_rnode = SPIFFS.openDir("/");
#endif
#if	defined(USE_LITTLEFS)
   fs_root = fs_rnode = LittleFS.openDir("/");
#endif
}

void flash_gc(void) {
#if	defined(USE_SPIFFS)
   SPIFFS.gc();
#endif
}

void flash_dir(void) {
#if	defined(USE_SPIFFS)
   Dir dir = SPIFFS.openDir("/");
#endif
#if	defined(USE_LITTLEFS)
   Dir dir = LittleFS.openDir("/");
#endif
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
