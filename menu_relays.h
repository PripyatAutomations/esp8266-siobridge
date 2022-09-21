
static const char *menu_setup_relays_help[] = {
   "********************\r\n"
   "* Relay/GPIO setup *\r\n",
   "********************\r\n"
   "\r\n",
   "X\tBack to GPIO menu\r\n",
   "Q\tBack to Setup menu\r\n",
   NULL
};

MenuItem menu_setup_relays[] = {
   { 'X', NULL, menu_relays },
   { 'Q', NULL, menu_setup },
   { 0,   NULL,  NULL }
};

static const char *menu_relays_help[] = {
   "***************\r\n",
   "* Relays/GPIO *\r\n",
   "***************\r\n",
   "\r\n",
   "L\tList\r\n",
   "S\tSetup\r\n",
   "\r\n",
   "Q\tBack to main menu\r\n",
   NULL
};
MenuItem menu_relays[] = {
   { 'L', cmd_relays_list, NULL },
   { 'S', NULL, menu_setup_relays },
   { 'Q', NULL, menu_main },
   { 0, NULL, NULL }
};
