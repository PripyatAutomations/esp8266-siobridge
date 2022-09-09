/*
 * OTA (Over The Air) Updates
 *
 * This supports OTA updates from the async webserver
 */
#include "siobridge.h"

void ota_setup(void) {
#if	defined(USE_SPIFFS)
        SPIFFS.end();
#endif
#if	defined(USE_LITTLEFS)
        LittleFS.end();
#endif
}

void ota_fail(void) {
   Serial.printf("* OTA Update Failed!\r\n");
}
