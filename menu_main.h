
static const char *menu_main_help[] = {
   "**********************************\r\n",
   "* esp8266-siobridge console help *\r\n",
   "**********************************\r\n\r\n",
   " C #\tConnect to port <#>\r\n",
   " I\tInformation screen\r\n",
   " R\tRelays/GPIO control\r\n",
   "\r\n",
   " S\tSetup Menu\r\n",
   "\r\n",
   "Q\tLogout\r\n",
   NULL
};

MenuItem menu_main[] = {
   { 'C', cmd_connect, NULL },
   { 'I', cmd_info, NULL },
   { 'R', NULL, menu_relays },
   { 'S', NULL, menu_setup },
   { 'Q', cmd_logout, NULL },
   { 0, NULL, NULL }
};
