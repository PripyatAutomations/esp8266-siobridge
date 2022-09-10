
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
static const char *menu_wifi_sta_help[] = {
   "****************\r\n"
   "* WiFi Station *\r\n",
   "****************\r\n",
   "\r\n",
   "A\tAdd AP\r\n",
   "D\tDelete AP\r\n",
   "L\tList APs (with passwords)\r\n",
   "\r\n",
   "Q\tBack to Wifi Menu\r\n",
   "X\tBack to Main Menu\r\n",
   NULL
};

MenuItem menu_wifi_sta[] = {
   { 'A', cmd_wifi_sta_add_ap, NULL },
   { 'D', cmd_wifi_sta_del_ap, NULL },
   { 'L', cmd_wifi_sta_list_aps, NULL },
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
   "C\tStation Mode\r\n",
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
   { 'C', cmd_wifi_sta, menu_wifi_sta },
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

static const char *menu_users_help[] = {
   "*******************\r\n",
   "* User Management *\r\n",
   "*******************\r\n",
   "\r\n",
   "A\tAdd User\r\n",
   "D\tDelete User\r\n",
   "L\tList Users\r\n",
   "\r\n",
   "X\tMain Menu\r\n",
   NULL
};

MenuItem menu_users[] = {
   { 'A', cmd_user_add, NULL },
   { 'D', cmd_user_delete, NULL },
   { 'L', cmd_user_list, NULL },
   { 'X', NULL, menu_main },
   { 0, NULL, NULL }
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
   "U\tUser Management\r\n",
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
   { 'U', NULL, menu_users },
   { 'W', NULL, menu_wifi },
   { 'X', NULL, menu_main },
   { 0, NULL, NULL },
};


//////////////////////////////

Menu menus[] = {
  { "main",  menu_main, menu_main_help },
  { "setup", menu_setup, menu_setup_help },
  { "users", menu_users, menu_users_help },
  { "wifi", menu_wifi, menu_wifi_help },
  { "wifi_ap", menu_wifi_ap, menu_wifi_ap_help },
  { "wifi_sta", menu_wifi_sta, menu_wifi_sta_help },
  { NULL, NULL, NULL }
};
