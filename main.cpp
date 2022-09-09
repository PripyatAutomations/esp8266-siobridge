/*
 * Here we deal with the main loop stuff
 *
 * Try to be asynchronous where possible as this is a fast uc...
 */
#include "siobridge.h"
int RESET_PIN = 0; // = GPIO0 on nodeMCU

void main_setup(void) {
   /* enable reset pin */
   pinMode(RESET_PIN, INPUT_PULLUP);

   /* Setup serial console */
   Serial.setRxBufferSize(1024);
   Serial.begin(115200);

   Serial.printf("\r\n\r\n\r\n\r\n\r\nBOOTWAIT ");
   for (int i = 5; i > 0; i--) {
       Serial.printf("%d... ", i);
       delay(1000);
   }
   Serial.printf("\r\n\r\nesp8266-siobridge %s (%s) starting\r\n\r\n", VERSION, ESP.getSketchMD5());
   Serial.printf("ChipID: %lu CoreVer: %s SDKVer: %s\r\n", ESP.getChipId(), ESP.getCoreVersion(), ESP.getSdkVersion());
   Serial.printf("CPUFreq: %dMHz\r\n", ESP.getCpuFreqMHz());
   Serial.printf("Last Reset Reason: %s\r\n", ESP.getResetReason());
   Serial.printf("SketchSize: %lu Free space: %lu HeapFrag: %lu MaxFreeBlock: %lu\r\n", ESP.getSketchSize(), ESP.getFreeSketchSpace(), ESP.getHeapFragmentation(), ESP.getMaxFreeBlockSize());
   flash_init();

   /* If loading config is successful, start wifi */
   if (config_load() == true) {
      wifi_setup();
   } else { /* Present a fallback AP mode, so maybe user can fix it */
      wifi_failsafe();
   }
}

void main_loop(void) {
   wifi_loop();
}
