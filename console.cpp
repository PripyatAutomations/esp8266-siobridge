/*
 * provide a simple console for setting up the device and chosing which port to connect
 *
 * Eventually we will clean this up more...
 */
#include "siobridge.h"

#include "cmd_admin.h"
#include "cmd_wifi.h"
#include "cmd_user.h"
#include "cmd_port.h"
#include "menu_layout.h"
const char *s_connected = "connected";
const char *s_disconnected = "disconencted";
const char *s_redacted = "*****";

const char *sio_connected(int port) {
   if (cfg.ports[port].refcnt > 0)
      return s_connected;

   return s_disconnected;
}

unsigned int sio_baud(int port) {
   return cfg.ports[port].baud_rate;
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
      return s_redacted;
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
      ch->printf("show_menu(%s) failed.\r\n", menu);
      return false;
   }

   int i = 0;
   while (mp->help[i] != NULL) {
      ch->printf("%s\r\n", mp->help[i]);
      i++;
   }
   console_prompt(ch, menu);
   return true;
}
