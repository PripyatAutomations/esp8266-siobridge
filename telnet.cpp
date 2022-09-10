/*
 * Support for telnet console and telnet-per-port
 */
#include "siobridge.h"

//#define MAX_SRV_CLIENTS 1
WiFiServer server(23);
WiFiClient serverClient;

void AcceptConnection(void) {
   if (serverClient && serverClient.connected()) 
      serverClient.stop();

   serverClient = server.available();
   serverClient.write("esp8266-siobridge ");
   serverClient.write(VERSION);
   serverClient.write(" ready!\n");
//   show_menu(serverClient, "main");
//   console_prompt(serverClient, "Main> ");
}

void ManageConnected(void) {
   size_t rxlen = serverClient.available();

   if (rxlen > 0) {
      uint8_t sbuf[rxlen];
      serverClient.readBytes(sbuf, rxlen);
      Serial.write(sbuf, rxlen);
    }
	
    size_t txlen = Serial.available();

    if (txlen > 0) {
       uint8_t sbuf[txlen];
       Serial.readBytes(sbuf, txlen);
       serverClient.write(sbuf, txlen);
    }
}

void telnet_loop(void) {
   /* Accept new pending telnet clients */
   if (server.hasClient())
      AcceptConnection();
   else if (serverClient && serverClient.connected()) {
      /* XXX: Add multi-user support */
      /* Handle already connected telnet users */
      ManageConnected();
   }
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
      // push UART data to all connected telnet clients
      if (serverClient && serverClient.connected()) {
         serverClient.write(sbuf, len);
      }
   }
}

void telnet_setup(void) {
   server.begin();
   server.setNoDelay(true);
}

void telnet_stop(void) {
   server.stop();
}
