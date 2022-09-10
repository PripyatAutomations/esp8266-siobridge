/*
 * provide a simple console for setting up the device and chosing which port to connect
 *
 * Eventually we will clean this up more...
 */
#include "siobridge.h"
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

   for (int i = 0; i < (MAX_PORTS - 1); i++)
      ch->printf("\tsio%d: %s <%s@%d>\n", i, sio_connected(i), sio_unread_buffers(i), sio_baud(i));

   ch->printf("\r\n");
   ch->printf("WiFi Status:\r\n");
   if (cfg.wifi_mode == WIFI_AP || cfg.wifi_mode == WIFI_AP_STA)
      ch->printf("\tAP - SSID: \"%s\" PASS: \"%s\"\r\n", cfg.wifi_ap_ssid, redact_password(cfg.wifi_ap_pass));
//   if (cfg.wifi_mode == WIFI_STA || cfg.wifi_mode == WIFI_AP_STA)
//   ch->printf("\tSTA - SSID: \"%s\" PASS: \"%s\"\r\n", cfg.wifi_sta_ssid, redact_password(cfg.wifi_sta_pass));
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

static bool cmd_wifi_sta(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_staent(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_syslog(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_ap_pass(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_ap_ssid(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_ap_timeout(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_sta_add_ap(Stream *ch, const char *args[]) {
   wifi_add_ap(args[0], args[1]);
}

static bool cmd_wifi_sta_del_ap(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_sta_list_aps(Stream *ch, const char *args[]) {
}

static bool cmd_logout(Stream *ch, const char *args[]) {
}

static bool cmd_user_add(Stream *ch, const char *args[]) {
   user_t *up = user_add(args[0], args[1]);

   if (up != NULL) {
      return true;
   }
   return false;
}

static bool cmd_user_list(Stream *ch, const char *args[]) {
   user_t *u;

   for (int i = 0; i < MAX_USERS; i++) {
      u = &users[i];

      if (u->user[0] != '\0')
         ch->printf("User<%d>: %s <%s> %sabled\r\n", i, u->user, privilege_str(u), (u->disabled == false ? "en" : "dis"));
   }
}

static bool cmd_user_delete(Stream *ch, const char *args[]) {
   return user_delete(args[0]);
}

static bool cmd_port_add(Stream *ch, const char *args[]) {
}
static bool cmd_port_delete(Stream *ch, const char *args[]) {
}

static bool cmd_port_list(Stream *ch, const char *args[]) {
}

#include "menu_layout.h"

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
   console_prompt(ch, menu);
   return true;
}
