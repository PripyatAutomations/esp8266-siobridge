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

   Serial.print("\r\n\r\n\r\n\r\n\r\nBOOTWAIT ");
   for (int i = 5; i > 0; i--) {
       Serial.printf("%d... ", i);
       delay(1000);
   }
   Serial.print("\r\n\r\n");
   Serial.printf("esp8266-siobridge %s starting\r\n\r\n", VERSION);
   flash_init();

   if (config_load() == true) {
      wifi_setup();
   } else { /* Present a fallback AP mode, so maybe user can fix it */
      wifi_failsafe();
   }
}

void main_loop(void) {
   wifi_loop();
}
