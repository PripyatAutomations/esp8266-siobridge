#if	!defined(_SIOBRIDGE_cfg_ports_h)
#define	_SIOBRIDGE_cfg_ports_h

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
   /*    KEY   		 TYPE   FUNC   		IVAL  		   CVAL   	BVAL 			FVAL*/
   { "console",		T_FUNC, cfg_port_console, NULL,		NULL,		NULL,			NULL },
   { "mode",		T_FUNC, cfg_port_mode,	NULL,		NULL,		NULL,			NULL },
   { "rx",		T_FUNC,	cfg_port_rx,	NULL,		NULL,		NULL,			NULL },
   { "telnet",		T_FUNC, cfg_port_telnet, NULL,		NULL,		NULL,			NULL },
   { "transport",	T_FUNC, cfg_port_transport, NULL,	NULL,		NULL,			NULL },
   { "trusted",		T_FUNC, cfg_port_trusted, NULL,		NULL,		NULL,			NULL },
   { "tx",		T_FUNC,	cfg_port_tx,	NULL,		NULL,		NULL,			NULL },
   { NULL,		T_NONE,	NULL,		NULL,		NULL,		NULL,			NULL }
};

#endif
