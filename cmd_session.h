#if	!defined(_SIOBRIDGE_cmd_session_h)
#define	_SIOBRIDGE_cmd_session_h

static bool cmd_logout(Stream *ch, const char *args[]) {
   session_t *sess = session_find(ch);

   if (!sess)
      return false;

   ch->printf("Goodbye!\r\n");
   session_end(sess);
}

#endif
