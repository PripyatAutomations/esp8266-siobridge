/*
 * Here we handle relaying incoming data to desired destinations
 *
 * IE: sio0 to sio1 or sio1 to telnet
 */
#include "siobridge.h"

void forwarding_setup(void) {
}

void forwarding_loop(void) {
#if	0
    //check clients for data
    //get data from the telnet client and push it to the UART
    while (serverClient.available()) {
      Serial.write(serverClient.read());
    }

    //check UART for data
    if (Serial.available()) {
      size_t len = Serial.available();
      uint8_t sbuf[len];
      Serial.readBytes(sbuf, len);
      //push UART data to all connected telnet clients
      if (serverClient && serverClient.connected()) {
        serverClient.write(sbuf, len);
      }
    }
#endif
}
