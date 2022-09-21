static const char *menu_ports_help[] = {
  "**************\r\n",
  "* Port Setup *\r\n"
  "**************\r\n",
  "\r\n",
  "A\tAdd Port\r\n",
  "D\tDelete Port\r\n",
  "L\tList Ports\r\n",
  "\r\n",
  "Q\tBack to setup menu\r'n",
  NULL
};

MenuItem menu_ports[] = {
   { 'A', cmd_port_add, NULL },
   { 'D', cmd_port_delete, NULL },
   { 'L', cmd_port_list, NULL },
   { 'Q', NULL, menu_setup },
   { 0, NULL, NULL }
};
