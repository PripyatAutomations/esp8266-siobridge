static const char *menu_wifi_ap_help[] = {
   "***********\r\n",
   "* WiFi AP *\r\n",
   "***********\r\n",
   " This menu allows you to configure the Access Point mode.\r\n",
   " AP mode is only active if wifi_mode is 'both' or 'ap'\r\n",
   "\r\n",
   "S\tSSID\r\n",
   "P\tPassword\r\n",
   "T\tTimeout\r\n",
   "\r\n",
   "X\tBack to Wifi Menu\r\n",
   "Q\tBack to setup menu\r\n",
   NULL
};

MenuItem menu_wifi_ap[] = {
   { 'P', cmd_wifi_ap_pass, NULL },
   { 'S', cmd_wifi_ap_ssid, NULL },
   { 'T', cmd_wifi_ap_timeout, NULL },
   { 'Q', NULL, menu_setup },
   { 'X', NULL, menu_wifi },
   { 0, NULL, NULL },
};

//////////////////////////////
static const char *menu_wifi_sta_help[] = {
   "****************\r\n"
   "* WiFi Station *\r\n",
   "****************\r\n",
   " The siobridge can store up to ",
   STRINGIFY(MAX_APS),
   " access points and auto connect to the strongest one.",
   "\r\n",
   "A\tAdd AP\r\n",
   "D\tDelete AP\r\n",
   "L\tList APs (with passwords)\r\n",
   "\r\n",
   "X\tBack to Wifi Menu\r\n",
   "Q\tBack to setup menu\r\n",
   NULL
};

MenuItem menu_wifi_sta[] = {
   { 'A', cmd_wifi_sta_add_ap, NULL },
   { 'D', cmd_wifi_sta_del_ap, NULL },
   { 'L', cmd_wifi_sta_list_aps, NULL },
   { 'Q', NULL, menu_setup },
   { 'X', NULL, menu_wifi },
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
   "\r\n",
   "Q\tBack to setup menu\r\n",
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
   { 'Q', NULL, menu_setup },
   { 0, NULL, NULL },
};
