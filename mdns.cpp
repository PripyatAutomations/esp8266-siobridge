/*
 * basic mDNS service for making it easier to find esp-siobridge
 */
#include "siobridge.h"
#include "mdns.h"
#if	0
// When an mDNS packet gets parsed this callback gets called.
void mdns_packet_cb(const mdns::MDns* packet){
  packet->Display();
  packet->DisplayRawPacket();
}

// When an mDNS packet gets parsed this callback gets called once per Query.
// See mdns.h for definition of mdns::Query.
void mdns_query_cb(const mdns::Query* query){
  query->Display();
}

// When an mDNS packet gets parsed this callback gets called once per Query.
// See mdns.h for definition of mdns::Answer.
void mdns_answer_cb(const mdns::Answer* answer){
  answer->Display();
}

// Initialise MDns. If you don't want the optioanl callbacks, just provide a NULL pointer.
mdns::MDns my_mdns(mdns_packet_cb, mdns_query_cb, mdns_answer_cb);

void mdns_setup(void) {
   my_mdns.begin(); // call to startUdpMulticast
}

unsigned int last_packet_count = 0;

void mdns_loop(void) {
   my_mdns.loop();

#ifdef DEBUG_STATISTICS
  // Give feedback on the percentage of incoming mDNS packets that fitted in buffer.
  // Useful for tuning the buffer size to make best use of available memory.
  if (last_packet_count != my_mdns.packet_count && my_mdns.packet_count != 0){
     last_packet_count = my_mdns.packet_count;
     Serial.print("mDNS decode success rate: ");
     Serial.print(100 - (100 * my_mdns.buffer_size_fail / my_mdns.packet_count));
     Serial.print("%\nLargest packet size: ");
     Serial.println(my_mdns.largest_packet_seen);
  }
#endif
}

void mdns_stop(void) {
 /* */
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
#endif
