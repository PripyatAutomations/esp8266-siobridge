#if	!defined(_SIOBRIDGE_sessions_h)
#define	_SIOBRIDGE_sessions_h

typedef struct session session_t;
struct session {
   user_t *user;
   Stream *chan;
   bool   active_ports[MAX_PORTS];	/* set to true on any ports the session is using */
};
extern session_t sessions[MAX_SESSIONS];
extern bool session_attach_port(session_t *session, sio_port_t *port);
extern bool session_detach_port(session_t *session, sio_port_t *port);
extern session_t *session_start(Stream *ch, user_t *u);

#endif
