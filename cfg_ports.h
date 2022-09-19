#if	!defined(_SIOBRIDGE_cfg_ports_h)
#define	_SIOBRIDGE_cfg_ports_h
sio_port_t ports[MAX_PORTS];

bool cfg_port_console(const char *key, const char *val, int line) {
   return false;
}


bool cfg_port_mode(const char *key, const char *val, int line) {
   return false;
}

bool cfg_port_rx(const char *key, const char *val, int line) {
   return false;
}


bool cfg_port_tx(const char *key, const char *val, int line) {
   return false;
}


bool cfg_port_telnet(const char *key, const char *val, int line) {
   return false;
}

bool cfg_port_transport(const char *key, const char *val, int line) {
   return false;
}

bool cfg_port_trusted(const char *key, const char *val, int line) {
   return false;
}

config_item_t config_items_ports[] = {
   /*    KEY   		 TYPE   FUNC  			DUMP 		IVAL  		   CVAL   	BVAL 			FVAL*/
   { "console",		T_BOOL, NULL,		 	NULL,		NULL,		NULL,		NULL,			NULL },
   { "mode",		T_FUNC, NULL,			NULL,		NULL,		NULL,		NULL,			NULL },
   { "rx",		T_INT,	NULL,			NULL,		NULL,		NULL,		NULL,			NULL },
   { "telnet",		T_INT,  NULL,	 		NULL,		NULL,		NULL,		NULL,			NULL },
   { "transport",	T_CHAR, NULL,		 	NULL,		NULL,		NULL,		NULL,			NULL },
   { "trusted",		T_BOOL, NULL,	 		NULL,		NULL,		NULL,		NULL,			NULL },
   { "tx",		T_INT,	NULL,			NULL,		NULL,		NULL,		NULL,			NULL },
   { NULL,		T_NONE,	NULL,			NULL,		NULL,		NULL,		NULL,			NULL }
};

config_item_t *mutate_ports_ci(config_item_t *ci, int pn) {
   config_item_t *d = NULL;
   

   Serial.printf("mutate_ports_ci<%x, %d>\r\n", ci, pn);

   if (pn < 0 || pn > MAX_APS)
      return NULL;

   if (ports[pn].ci != NULL)
      return ports[pn].ci;

   ports[pn].ci = ci;

   if ((d = (config_item_t *)malloc(sizeof(ci))) == NULL) {
      Serial.printf("OOM in mutate_ports_ci()! :(\r\n");
      abort();
   }

   for (int i = 0; ci[i].key != NULL; i++) {
      if (strncasecmp(ci[i].key, "console", 7) == 0) {
         d[i].bval = &ports[pn].console;
      } else if (strncasecmp(ci[i].key, "enabled", 7) == 0) {
         d[i].bval = &ports[pn].enabled;
      } else if (strncasecmp(ci[i].key, "rx", 2) == 0) {
         *d[i].ival = ports[pn].rx_pin;
      } else if (strncasecmp(ci[i].key, "telnet", 6) == 0) {
         *d[i].ival = ports[pn].telnet_port;
      } else if (strncasecmp(ci[i].key, "transport", 9) == 0) {
         /* XXX: nyi */
      } else if (strncasecmp(ci[i].key, "trusted", 7) == 0) {
         d[i].bval = &ports[pn].trusted;
      } else if (strncasecmp(ci[i].key, "tx", 2) == 0) {
         *d[i].ival = ports[pn].tx_pin;
      }
   }
   return NULL;
}

#endif
