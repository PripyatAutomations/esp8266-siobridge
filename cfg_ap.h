#if	!defined(_SIOBRIDGE_cfg_ap_h)
#define	_SIOBRIDGE_cfg_ap_h

config_item_t config_items_ap[] = {
   /*    KEY   		 TYPE   FUNC   	DUMP	IVAL  		   CVAL   	BVAL 			FVAL*/
   { "admin_user",	T_CHAR,	NULL,	NULL,	NULL,		   cfg.admin_user, NULL,		NULL },
};
#endif
