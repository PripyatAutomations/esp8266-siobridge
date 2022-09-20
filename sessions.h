#if	!defined(_SIOBRIDGE_sessions_h)
#define	_SIOBRIDGE_sessions_h

class SIOsession {
   private:
      SIOuser *user;
      /* This is how we decide which muxes to teardown */
      bool   active_ports[MAX_PORTS];	/* set to true on any ports the session is using */
      SIOsession *sessions[MAX_SESSIONS];
   public:
      Stream *chan;
      bool AttachPort(sio_port_t *port);
      bool DetachPort(sio_port_t *port);
      SIOsession(Stream *ch, SIOuser *u);
      bool End(void);
};
extern SIOsession *session_find(Stream *ch);

#endif
