#if	!defined(_SIOBRIDGE_cmd_user_h)
#define	_SIOBRIDGE_cmd_user_h

static bool cmd_user_add(Stream *ch, const char *args[]) {
   user_t *up = user_add(args[0], args[1]);

   if (up != NULL) {
      return true;
   }
   return false;
}

static bool cmd_user_list(Stream *ch, const char *args[]) {
   user_t *u;

   for (int i = 0; i < MAX_USERS; i++) {
      u = &users[i];

      if (u->user[0] != '\0')
         ch->printf("User<%d>: %s <%s> %sabled\r\n", i, u->user, privilege_str(u), (u->disabled == false ? "en" : "dis"));
   }
}

static bool cmd_user_delete(Stream *ch, const char *args[]) {
   return user_delete(args[0]);
}

#endif
