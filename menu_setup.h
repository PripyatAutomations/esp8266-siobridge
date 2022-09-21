static const char *menu_setup_help[] = {
   "***************************\r\n",
   "* esp8266-siobridge setup *\r\n",
   "***************************\r\n",
   "\r\n",
   "P\tPort setup\r\n",
   "W\tWiFi setup\r\n",
   "\r\n",
   "S\tSave configuration\r\n",
   "* admin tasks *\r\n",
   "U\tUser Management\r\n",
   "R\tRestart bridge\r\n",
   "\r\n",
   "Q\tBack to main menu\r\n",
   NULL
};

MenuItem menu_setup[] = {
   { 'P', NULL, menu_ports },
   { 'R', cmd_restart, NULL },
   { 'S', cmd_save_config, NULL },
   { 'U', NULL, menu_users },
   { 'W', NULL, menu_wifi },
   { 'Q', NULL, menu_main },
   {   0, NULL, NULL },
};
