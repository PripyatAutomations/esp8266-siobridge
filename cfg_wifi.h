#if	!defined(_SIOBRIDGE_cfg_wifi_h)
#define	_SIOBRIDGE_cfg_wifi_h

config_item_t config_items_wifi[] = {
   /*    KEY   	TYPE   FUNC   	DUMP	IVAL  	CVAL   	BVAL 	FVAL*/
   { "dhcp",	T_BOOL, NULL,	NULL,	NULL,	NULL,	NULL,	NULL },
   { "enabled",	T_BOOL,	NULL,	NULL,	NULL,	NULL, 	NULL,	NULL },
   { "gw",	T_CHAR, NULL,	NULL,	NULL,	NULL,	NULL,	NULL },
   { "ip",	T_CHAR, NULL,   NULL,	NULL,	NULL,	NULL,	NULL },
   { "mask",	T_CHAR, NULL,	NULL,	NULL,	NULL,	NULL,	NULL },
   { "pass",	T_CHAR,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL },
   { "ssid",	T_CHAR, NULL,	NULL,	NULL,	NULL,	NULL,	NULL },
   { NULL,	T_NONE, NULL,	NULL,	NULL,	NULL,	NULL,	NULL }
};

config_item_t *mutate_wifi_ci(config_item_t *ci, int apn) {
   config_item_t *d = NULL;
   

   Serial.printf("mutate_wifi_ci<%x, %d>\r\n", ci, apn);

   if (apn < 0 || apn > MAX_APS)
      return NULL;

   if ((d = (config_item_t *)malloc(sizeof(ci))) == NULL) {
      Serial.printf("OOM in mutate_wifi_ci()! :(\r\n");
      abort();
   }

   for (int i = 0; ci[i].key != NULL; i++) {
      if (strncasecmp(ci[i].key, "dhcp", 4) == 0) {
         d[i].bval = &aps[apn].dhcp;
      } else if (strncasecmp(ci[i].key, "enabled", 7) == 0) {
         d[i].bval = &aps[apn].enabled;
      } else if (strncasecmp(ci[i].key, "gw", 2) == 0) {
         d[i].cval = aps[apn].s_gw;
         /* XXX: convert to IPAddress */
      } else if (strncasecmp(ci[i].key, "ip", 2) == 0) {
         d[i].cval = aps[apn].s_ip;
         /* XXX: convert to IPAddress */
      } else if (strncasecmp(ci[i].key, "mask", 4) == 0) {
         d[i].cval = aps[apn].s_mask;
      } else if (strncasecmp(ci[i].key, "pass", 4) == 0) {
         d[i].cval = aps[apn].pass;
      } else if (strncasecmp(ci[i].key, "ssid", 4) == 0) {
         d[i].cval = aps[apn].ssid;
      }
   }
   return NULL;
}

#endif
