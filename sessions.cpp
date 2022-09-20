/*
 * Session management
 *
 * This handles connects/disconnects of clients and the resources they are using.
 *
 */
#include "siobridge.h"

session_t *session_start(Stream *ch, SIOuser *u) {
   session_t *s = (session_t *)malloc(sizeof(session_t));
   memset(s, 0, sizeof(0));

   /* A user has succesfully authenticated, start their session */
   u->refcnt++;
   s->user = u;
   s->chan = ch;
   return s;
}

bool session_end(session_t *s) {
   s->user->refcnt--;

   /* XXX: Close active ports */
   for (int i = 0; i < MAX_PORTS; i++) {
      session_detach_port(s, &cfg.ports[i]);
   }
   memset(s, 0, sizeof(session_t));
}

bool session_attach_port(session_t *session, sio_port_t *port) {
   return true;
}

bool session_detach_port(session_t *session, sio_port_t *port) {
   return true;
}

session_t *session_find(Stream *ch) {
   for (int i = 0; i < MAX_SESSIONS; i++) {
      if (sessions[i].chan == ch)
         return &sessions[i];
   }
   return NULL;
}
