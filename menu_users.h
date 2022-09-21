
static const char *menu_users_help[] = {
   "*******************\r\n",
   "* User Management *\r\n",
   "*******************\r\n",
   "\r\n",
   "A\tAdd User\r\n",
   "D\tDelete User\r\n",
   "L\tList Users\r\n",
   "\r\n",
   "Q\tBack to setup menu\r\n",
   NULL
};

MenuItem menu_users[] = {
   { 'A', cmd_user_add, NULL },
   { 'D', cmd_user_delete, NULL },
   { 'L', cmd_user_list, NULL },
   { 'Q', NULL, menu_setup },
   { 0, NULL, NULL }
};
