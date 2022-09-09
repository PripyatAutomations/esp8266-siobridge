/*
 * OTA (Over The Air) Updates
 */
#include "siobridge.h"

void ota_begin(void) {
#if	defined(USE_SPIFFS)
        SPIFFS.end();
#endif
#if	defined(USE_LITTLEFS)
        LittleFS.end();
#endif
}
