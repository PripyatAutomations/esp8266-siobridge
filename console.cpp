/*
 * provide a simple console for setting up the device and chosing which port to connect
 *
 */
#include "siobridge.h"
extern "C" {
  static const char *s_connected = "connected";
  static const char *s_disconnected = "disconencted";
  const char *redacted_pass = "*****";

  const char *sio_connected(int port) {
     if (cfg.ports[port].refcnt > 0)
        return s_connected;

     return s_disconnected;
  }

  unsigned int sio_baud(int port) {
     return  cfg.ports[port].baud_rate;
  }

  unsigned int sio_unread_buffers(int port) {
     return cfg.ports[port].unread_buffers;
  }

  const char *sio_bits(int port) {
     /* yuck... this is going to be big... */
     return "8N1";
  }

  ///////////////////////////////////////////
  /* Internal bits below here, Not exposed */
  ///////////////////////////////////////////
  static bool cmd_connect(Stream *ch, const char *args[]) {
  }

  /*
   * cmd_info: System information display
   */
  static bool cmd_info(Stream *ch, const char *args[]) {
     ch->printf("**************************\r\n");
     ch->printf("* esp8266-siobridge info *\r\n");
     ch->printf("**************************\r\n");
     ch->printf("\r\n");
     ch->printf("Serial Ports:\r\n");
  /* XXX: Make this just loop over the sio_ports array */
     ch->printf("\tsio0: %s <%s@%d>\n", sio_connected(0), sio_unread_buffers(0), sio_baud(0));
     ch->printf("\tsio1: %s <%s@%d>\n", sio_connected(1), sio_unread_buffers(1), sio_baud(1));
     ch->printf("\tsio2: %s <%s@%d>\n", sio_connected(2), sio_unread_buffers(2), sio_baud(2));
     ch->printf("\tsio3: %s <%s@%d>\n", sio_connected(3), sio_unread_buffers(3), sio_baud(3));
     ch->printf("\tsio4: %s <%s@%d>\n", sio_connected(4), sio_unread_buffers(4), sio_baud(4));
     ch->printf("\tsio5: %s <%s@%d>\n", sio_connected(5), sio_unread_buffers(5), sio_baud(5));
     ch->printf("\r\n");
     ch->printf("WiFi Setup:\r\n");
     ch->printf("\tAP - SSID: \"%s\" PASS: \"%s\"\r\n", cfg.wifi_ap_ssid, redact_password(cfg.wifi_ap_pass));
     /* XXX: print client networks */
  }


  static bool cmd_restart(Stream *ch, const char *args[]) {
     ch->println("* Restarting!");
     ESP.restart();
  }

  static bool cmd_wifi_dhcp(Stream *ch, const char *args[]) {
  }

  static bool cmd_wifi_dns(Stream *ch, const char *args[]) {
  }

  static bool cmd_wifi_ip(Stream *ch, const char *args[]) {
  }

  static bool cmd_wifi_gateway(Stream *ch, const char *args[]) {
  }

  static bool cmd_wifi_netmask(Stream *ch, const char *args[]) {
  }

  static bool cmd_wifi_ntp(Stream *ch, const char *args[]) {
  }

  static bool cmd_wifi_ap(Stream *ch, const char *args[]) {
  }

  static bool cmd_wifi_cli(Stream *ch, const char *args[]) {
  }

  static bool cmd_wifi_client(Stream *ch, const char *args[]) {
  }

  static bool cmd_wifi_syslog(Stream *ch, const char *args[]) {
  }

  static bool cmd_wifi_ap_pass(Stream *ch, const char *args[]) {
  }

  static bool cmd_wifi_ap_ssid(Stream *ch, const char *args[]) {
  }

  static bool cmd_wifi_ap_timeout(Stream *ch, const char *args[]) {
  }

  static bool cmd_wifi_cli_add_ap(Stream *ch, const char *args[]) {
     wifi_add_ap(args[0], args[1]);
  }

  static bool cmd_wifi_cli_del_ap(Stream *ch, const char *args[]) {
  }

  static bool cmd_wifi_cli_list_aps(Stream *ch, const char *args[]) {
  }

  static bool cmd_logout(Stream *ch, const char *args[]) {
  }

  //////////////////////////////
  // Menu Structure and Helps //
  //////////////////////////////
  typedef struct MenuItem MenuItem;
  struct MenuItem {
     const char key;
     bool (*func)(Stream *ch, const char *args[]);
     const MenuItem *submenu;
  };

  typedef struct Menu Menu;
  struct Menu {
     char name[16];
     MenuItem *menu;
     const char **help;
  };

  extern MenuItem menu_setup[], menu_wifi[];
  static const char *menu_main_help[] = {
     "**********************************\r\n",
     "* esp8266-siobridge console help *\r\n",
     "**********************************\r\n\r\n",
     " C <#>\tConnect to port <#>\r\n",
     " I    \tInformation screen\r\n",
     " S    \tSetup Menu\r\n",
     NULL
  };

  MenuItem menu_main[] = {
     { 'C', cmd_connect, NULL },
     { 'I', cmd_info, NULL },
     { 'S', NULL, menu_setup },
     { 'X', cmd_logout, NULL },
     { 0, NULL, NULL }
  };


  //////////////////////////////
  static const char *menu_wifi_ap_help[] = {
     "***********\r\n",
     "* WiFi AP *\r\n",
     "***********\r\n",
     "\r\n",
     "S\tSSID\r\n",
     "P\tPassword\r\n",
     "T\tTimeout\r\n",
     "\r\n",
     "Q\tBack to Wifi Menu\r\n",
     "X\tBack to Main Menu\r\n",
     NULL
  };

  MenuItem menu_wifi_ap[] = {
     { 'P', cmd_wifi_ap_pass, NULL },
     { 'S', cmd_wifi_ap_ssid, NULL },
     { 'T', cmd_wifi_ap_timeout, NULL },
     { 'Q', NULL, menu_wifi },
     { 'X', NULL, menu_main },
     { 0, NULL, NULL },
  };



  //////////////////////////////
  static const char *menu_wifi_cli_help[] = {
     "***************\r\n"
     "* WiFi Client *\r\n",
     "***************\r\n",
     "\r\n",
     "A\tAdd AP\r\n",
     "D\tDelete AP\r\n",
     "L\tList APs (with passwords)\r\n",
     "\r\n",
     "Q\tBack to Wifi Menu\r\n",
     "X\tBack to Main Menu\r\n",
     NULL
  };

  MenuItem menu_wifi_cli[] = {
     { 'A', cmd_wifi_cli_add_ap, NULL },
     { 'D', cmd_wifi_cli_del_ap, NULL },
     { 'L', cmd_wifi_cli_list_aps, NULL },
     { 'Q', NULL, menu_wifi },
     { 'X', NULL, menu_main },
     { 0, NULL, NULL },
  };

  //////////////////////////////
  static const char *menu_wifi_help[] = {
     "********\r\n",
     "* WiFi *\r\n",
     "********\r\n",
     "\r\n",
     "A\tAccess Point Mode\r\n",
     "C\tClient Mode\r\n",
     "\r\n",
     "* IP Config *\r\n",
     "D\tUse DHCP\r\n",
     "\r\n",
     "I\tIP address\r\n",
     "M\tNetmask\r\n",
     "G\tGateway\r\n",
     "\r\n",
     "* Service Hosts *\r\n",
     "T\tTime (NTP) servers\r\n",
     "L\tLog servers\r\n",
     "N\tDNS servers\r\n",
     NULL
  };

  MenuItem menu_wifi[] = {
     { 'A', cmd_wifi_ap, menu_wifi_ap, },
     { 'C', cmd_wifi_cli, menu_wifi_cli },
     { 'D', cmd_wifi_dhcp, NULL },
     { 'I', cmd_wifi_ip, NULL },
     { 'G', cmd_wifi_gateway, NULL },
     { 'L', cmd_wifi_syslog, NULL },
     { 'M', cmd_wifi_netmask, NULL },
     { 'N', cmd_wifi_dns, NULL },
     { 'T', cmd_wifi_ntp, NULL },
     { 'X', NULL, menu_main },
     { 0, NULL, NULL },
  };

  //////////////////////////////
  static const char *menu_setup_help[] = {
     "***************************\r\n",
     "* esp8266-siobridge setup *\r\n",
     "***************************\r\n",
     "\r\n",
     "* serial ports *\r\n",
     "0-F\tsio<#> setup (1-16)\r\n",
     "\r\n",
     "W\tWiFi setup\r\n",
     "\r\n",
     "* admin tasks *\r\n",
     "R\tRestart bridge\r\n",
     "X\tLeave setup menu\r\n",
     NULL
  };

  MenuItem menu_setup[] = {
     { '0', cmd_connect, NULL },
     { '1', cmd_connect, NULL },
     { '2', cmd_connect, NULL },
     { '3', cmd_connect, NULL },
     { '4', cmd_connect, NULL },
     { '5', cmd_connect, NULL },
     { '6', cmd_connect, NULL },
     { '7', cmd_connect, NULL },
     { '8', cmd_connect, NULL },
     { '9', cmd_connect, NULL },
     { 'A', cmd_connect, NULL },
     { 'B', cmd_connect, NULL },
     { 'C', cmd_connect, NULL },
     { 'D', cmd_connect, NULL },
     { 'E', cmd_connect, NULL },
     { 'F', cmd_connect, NULL },
     { 'R', cmd_restart, NULL },
     { 'W', NULL, menu_wifi },
     { 'X', NULL, menu_main },
     { 0, NULL, NULL },
  };


  //////////////////////////////

  Menu menus[] = {
    { "main",  menu_main, menu_main_help },
    { "setup", menu_setup, menu_setup_help },
    { "wifi", menu_wifi, menu_wifi_help },
    { "wifi_ap", menu_wifi_ap, menu_wifi_ap_help },
    { "wifi_cli", menu_wifi_cli, menu_wifi_cli_help },
    { NULL, NULL, NULL }
  };

  ///////////////////
  void menu_print(Stream *ch, const char *menu[]) {
     if (menu == NULL) {
        ch->printf("menu_print: NULL menu passed\r\n");
        return;
     }

     for (int i = 0; i < (sizeof(menu) / sizeof(menu[0])); i++) {
        if (menu[i] == NULL)
           return;

        ch->printf(menu[i]);
     }
  }

  /* Print a prompt and read the user input then return it */
  const char *console_prompt(Stream *ch, const char *prompt) {
      char s[256];
      size_t rb = 0;

      ch->printf("%s> ", prompt);
      rb = ch->readBytesUntil('\n', s, 255);
      s[rb] = '\0';

      if (rb <= 0)
         ch->printf("%s> ", prompt);

      ch->printf("\r\n");

      return s;
  }

  /* Simple mechanism to avoid showing passwords on the console, if needed */
  const char *redact_password(const char *p) {
     if (cfg.redact_passwords)
        return redacted_pass;
     return p;
  }

  /* display the menu - returns false if errors */
  bool show_menu(Stream *ch, const char *menu) {
     const Menu *mp = NULL;

     for (int i = 0; menus[i].name != NULL; i++) {
         if (strcasecmp(menu, menus[i].name) == 0) {
            mp = &menus[i];
            break;
         }
     }
     
     if (!mp) {
        Serial.printf("show_menu(%s) failed.\r\n", menu);
        return false;
     }

     int i = 0;
     while (mp->help[i] != NULL) {
        Serial.printf("%s\r\n", mp->help[i]);
        i++;
     }
     return true;
  }
/* extern "C" */
}
