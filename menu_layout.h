/*
 * ensure all menus are NULL terminated and listed in menus[]
 */
#if	!defined(_SIOBRIDGE_menu_layout_h)
#define	_SIOBRIDGE_menu_layout_h

extern MenuItem menu_main[], menu_setup[];
extern MenuItem menu_wifi[], menu_relays[];
#include "menu_wifi.h"
#include "menu_users.h"
#include "menu_ports.h"
#include "menu_setup.h"
#include "menu_relays.h"
#include "menu_fs.h"
#include "menu_main.h"

Menu menus[] = {
  { "main",  menu_main, menu_main_help },
  { "fs", menu_fs, menu_fs_help },
  { "ports", menu_ports, menu_ports_help },
  { "relays", menu_relays, menu_relays_help },
  { "relays_setup", menu_setup_relays, menu_setup_relays_help },
  { "setup", menu_setup, menu_setup_help },
  { "users", menu_users, menu_users_help },
  { "wifi", menu_wifi, menu_wifi_help },
  { "wifi_ap", menu_wifi_ap, menu_wifi_ap_help },
  { "wifi_sta", menu_wifi_sta, menu_wifi_sta_help },
  { NULL, NULL, NULL }
};

#endif
