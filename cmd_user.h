#if	!defined(_SIOBRIDGE_cmd_user_h)
#define	_SIOBRIDGE_cmd_user_h

static bool cmd_user_add(Stream *ch, const char *args[]) {
   int privs = PRIV_SUPERUSER;

   if (args[2] != NULL)
      privs = atoi(args[2]);

   if (privs < 0)
      privs = PRIV_SUPERUSER;

   SIOuser *up = new SIOuser(args[0], args[1], privs);

   if (up != NULL) {
      return true;
   }
   return false;
}

static bool cmd_user_list(Stream *ch, const char *args[]) {
   SIOuser *u = NULL;

   for (int i = 0; i < MAX_USERS; i++) {
      u = users[i];

      ch->printf("User<%d>: %s <%s> %sabled\r\n", i, u->GetUsername(), u->GetPrivs(), (u->disabled == false ? "en" : "dis"));
   }
}

static bool cmd_user_delete(Stream *ch, const char *args[]) {
   return user_delete(args[0]);
}

#endif
