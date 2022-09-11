/*
 * provide a simple console for setting up the device and chosing which port to connect
 *
 * Eventually we will clean this up more...
 */
#include "siobridge.h"

#include "cmd_admin.h"
#include "cmd_port.h"
#include "cmd_session.h"
#include "cmd_wifi.h"
#include "cmd_user.h"
#include "menu_layout.h"

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

bool show_help(Stream *ch, const char *menu) {
   const Menu *mp = NULL;

   for (int i = 0; menus[i].name != NULL; i++) {
       if (strcasecmp(menu, menus[i].name) == 0) {
          mp = &menus[i];
          break;
       }
   }
   
   if (!mp) {
      ch->printf("* show_help(%s) failed.\r\n", menu);
      return false;
   }

   int i = 0;
   while (mp->help[i] != NULL) {
      ch->printf("%s\r\n", mp->help[i]);
      i++;
   }
}

/* display the menu - returns false if errors */
bool show_menu(Stream *ch, const char *menu) {
   const Menu *mp = NULL;
   const char *r = NULL;

   for (int i = 0; menus[i].name != NULL; i++) {
       if (strcasecmp(menu, menus[i].name) == 0) {
          mp = &menus[i];
          break;
       }
   }
   
   if (!mp) {
      ch->printf("* show_menu(%s) failed.\r\n", menu);
      return false;
   }

   show_help(ch, menu);

   while(true) {
      r = console_prompt(ch, menu);

      if (r == NULL)
         break;
   }
   return true;
}

/* try to come up with a text representation of the address of the port */
const char *get_tty_from_stream(Stream *) {
   return NULL;
}
