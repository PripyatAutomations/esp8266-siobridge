/*
 * Stuff for parsing the [general] section of config.txt
 */
#if	!defined(_SIOBRIDGE_cfg_general_h)
#define	_SIOBRIDGE_cfg_general_h

bool cfg_wifi_phymode(const char *key, const char *val, int line) {
   switch(*val) {
      case 'b':
      case 'B':
         cfg.wifi_phymode = WIFI_PHY_MODE_11B;
         break;
      case 'g':
      case 'G':
         cfg.wifi_phymode = WIFI_PHY_MODE_11G;
         break;
      case 'n':
      case 'N':
         cfg.wifi_phymode = WIFI_PHY_MODE_11N;
         break;
      default:      
         Serial.printf("Invalid entry at config.txt:%d\r\n", line);
         break;
   }
}

bool cfg_dump_wifi_phymode(Stream *ch) {
   switch (cfg.wifi_phymode) {
      case WIFI_PHY_MODE_11B:
         ch->printf("wifi_phymode=b\r\n");
         break;
      case WIFI_PHY_MODE_11G:
         ch->printf("wifi_phymode=g\r\n");
         break;
      case WIFI_PHY_MODE_11N:
         ch->printf("wifi_phymode=n\r\n");
         break;
      default:
         break;
   }
}

bool cfg_dump_wifi_mode(Stream *ch) {
}

bool cfg_wifi_mode(const char *key, const char *val, int line) {
   if (strncasecmp(val, "both", 4) == 0) {
      cfg.wifi_mode = WIFI_AP_STA;
   } else if (strncasecmp(val, "sta", 3) == 0) {
      cfg.wifi_mode = WIFI_STA;
   } else if (strncasecmp(val, "ap", 2) == 0) {
      cfg.wifi_mode = WIFI_AP;
   } else
      Serial.printf("Invalid entry at config.txt:%d\r\n", line);
}

config_item_t config_items_general[] = {
   /*    KEY   		 TYPE   FUNC   	DUMP	IVAL  		   CVAL   	BVAL 			FVAL*/
   { "admin_user",	T_CHAR,	NULL,	NULL,	NULL,		   cfg.admin_user, NULL,		NULL },
   { "admin_pass",	T_CHAR, NULL,	NULL,	NULL,		   cfg.admin_pass, NULL,		NULL },
   { "ap_chan", 	T_INT,	NULL, 	NULL,	&cfg.wifi_ap_chan,  NULL, 	NULL,			NULL },
   { "ap_hidden",	T_BOOL, NULL, 	NULL,	NULL, 		   NULL, 	&cfg.wifi_ap_hidden, 	NULL },
   { "ap_pass",		T_CHAR, NULL, 	NULL,	NULL, 		   cfg.wifi_ap_pass, NULL,		NULL },
   { "ap_ssid", 	T_CHAR, NULL, 	NULL,	NULL, 		   cfg.wifi_ap_ssid, NULL,		NULL },
   { "ap_wait", 	T_INT,	NULL, 	NULL,	&cfg.wifi_ap_wait,  NULL, 	NULL,			NULL },
   { "dns", 		T_CHAR, NULL,	NULL,	NULL,		   cfg.dns_host, NULL,			NULL },
   { "hostname", 	T_CHAR, NULL, 	NULL,	NULL,		   cfg.hostname, NULL,			NULL },
   { "http_port", 	T_INT,	NULL, 	NULL,	&cfg.http_port,     NULL,	NULL,			NULL },
   { "redact_passwords",T_BOOL, NULL, 	NULL,	NULL, 		   NULL,	&cfg.redact_passwords,	NULL },
   { "ntp",		T_CHAR, NULL,	NULL,	NULL,	           cfg.ntp_host, NULL,			NULL },
   { "syslog",          T_CHAR, NULL,  	NULL,	NULL,		   cfg.syslog_host, NULL,		NULL },
   { "wifi_mac",	T_CHAR, NULL,	NULL,	NULL,		   cfg.wifi_mac, NULL,			NULL },
   { "wifi_mode",	T_FUNC, &cfg_wifi_mode, 
                                &cfg_dump_wifi_mode,
                                               NULL,		   NULL,	NULL,			NULL },
   { "wifi_phymode", 	T_FUNC, &cfg_wifi_phymode,
                                &cfg_dump_wifi_phymode,
                                                NULL, 	 	   NULL, 	NULL,			NULL },
   { "wifi_power", 	T_INT,	NULL, 	NULL,	NULL,		   NULL,	NULL,			&cfg.wifi_power },
   { "wifi_timeout",	T_INT, 	NULL, 	NULL,	&cfg.wifi_timeout, NULL,	NULL,			NULL },
   {  NULL, 		T_NONE, NULL, 	NULL,	NULL, 		   NULL,	NULL,			NULL }
};

#endif
