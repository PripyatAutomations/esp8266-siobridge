#include "siobridge.h"
const char *s_connected = "connected";
const char *s_disconnected = "disconencted";

typedef struct Menu Menu;
struct Menu {
   const char key;
   bool (*func)();
   Menu *submenu;
};

const char *sio_connected(int port) {
   if (port == 0)
      return s_connected;
   else
      return s_disconnected;
}

unsigned int sio_baud(int port) {
   return 115200;
}

unsigned int sio_unread_buffers(int port) {
   return 0;
}

const char *sio_bits(int port) {
   return "8N1";
}

bool cmd_connect(void) {
}

/*
 * cmd_info: System information display
 */
bool cmd_info(void) {
   Serial.println("**************************");
   Serial.println("* esp8266-siobridge info *");
   Serial.println("**************************");
   Serial.println("");
   Serial.println("Serial Ports:");
   Serial.printf("\tsio0: %s <%s@%d>\n", sio_connected(0), sio_unread_buffers(0), sio_baud(0));
   Serial.printf("\tsio1: %s <%s@%d>\n", sio_connected(1), sio_unread_buffers(1), sio_baud(1));
   Serial.printf("\tsio2: %s <%s@%d>\n", sio_connected(2), sio_unread_buffers(2), sio_baud(2));
   Serial.printf("\tsio3: %s <%s@%d>\n", sio_connected(3), sio_unread_buffers(3), sio_baud(3));
   Serial.printf("\tsio4: %s <%s@%d>\n", sio_connected(4), sio_unread_buffers(4), sio_baud(4));
   Serial.printf("\tsio5: %s <%s@%d>\n", sio_connected(5), sio_unread_buffers(5), sio_baud(5));
   Serial.println("");
}

bool cmd_main_help(void) {
   Serial.println("**********************************");
   Serial.println("* esp8266-siobridge console help *");
   Serial.println("**********************************");
   Serial.println("");
   Serial.printf(" C <#>\tConnect to channel <#>\n");
   Serial.printf(" I    \tInformation screen\n");
   Serial.printf(" S    \tSetup Menu\n");
}


bool cmd_restart(void) {
   Serial.println("* Restarting!");
}

bool cmd_wifi_dhcp(void) { }
bool cmd_wifi_dns(void) { }
bool cmd_wifi_ip(void) { }
bool cmd_wifi_gateway(void) { }
bool cmd_wifi_netmask(void) { }
bool cmd_wifi_ntp(void) { }
bool cmd_wifi_pass(void) { }
bool cmd_wifi_ssid(void) { }
bool cmd_wifi_syslog(void) { }

bool cmd_wifi(void) {
   Serial.println("********");
   Serial.println("* WiFi *");
   Serial.println("********");
   Serial.println("");
   Serial.println("S\tSSID");
   Serial.println("P\tPassword");
   Serial.println("");
   Serial.println("* IP Config *");
   Serial.println("D\tUse DHCP");
   Serial.println("");
   Serial.println("I\tIP address");
   Serial.println("M\tNetmask");
   Serial.println("G\tGateway");
   Serial.println("");
   Serial.println("* Service Hosts *");
   Serial.println("T\tTime (NTP) servers");
   Serial.println("L\tLog servers");
   Serial.println("N\tDNS servers");   
}

bool cmd_setup_help(void) {
   Serial.println("***************************");
   Serial.println("* esp8266-siobridge setup *");
   Serial.println("***************************");
   Serial.println("");
   Serial.println("* serial ports *");
   Serial.println("0\tsio0 setup");
   Serial.println("1\tsio1 setup");
   Serial.println("2\tsio2 setup");
   Serial.println("3\tsio3 setup");
   Serial.println("4\tsio4 setup");
   Serial.println("5\tsio5 setup");
   Serial.println("");
   Serial.println("W\tWiFi setup");
   Serial.println("");
   Serial.println("* admin tasks *");
   Serial.println("R\tRestart bridge");
   Serial.println("X\tLeave setup menu");
}

Menu menu_main[] = {
   { 'C', cmd_connect, NULL },
   { 'I', cmd_info, NULL },
   { 'S', cmd_setup_help, NULL },
   { 0, NULL, NULL }
};

Menu menu_wifi[] = {
   { 'D', cmd_wifi_dhcp, NULL },
   { 'I', cmd_wifi_ip, NULL },
   { 'G', cmd_wifi_gateway, NULL },
   { 'L', cmd_wifi_syslog, NULL },
   { 'M', cmd_wifi_netmask, NULL },
   { 'N', cmd_wifi_dns, NULL },
   { 'P', cmd_wifi_pass, NULL },
   { 'S', cmd_wifi_ssid, NULL },
   { 'T', cmd_wifi_ntp, NULL },
   { 0, NULL, NULL },
};
