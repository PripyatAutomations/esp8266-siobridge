/*
 * Session management
 *
 * This handles connects/disconnects of clients and the resources they are using.
 *
 */
#include "siobridge.h"

SIOsession *sessions[MAX_SESSIONS];

SIOsession::SIOsession(Stream *ch, SIOuser *u) {
   /* A user has succesfully authenticated, start their session */
   this->user = u;
   this->user->refcnt++;
   this->chan = ch;
}

bool SIOsession::End(void) {
   this->user->refcnt--;

   /* XXX: Close active ports */
   for (int i = 0; i < MAX_PORTS; i++) {
      this->DetachPort(&cfg.ports[i]);
   }
}

bool SIOsession::AttachPort(sio_port_t *port) {
   return true;
}

bool SIOsession::DetachPort(sio_port_t *port) {
   return true;
}

SIOsession *session_find(Stream *ch) {
   for (int i = 0; i < MAX_SESSIONS; i++) {
      if (sessions[i]->chan == ch)
         return sessions[i];
   }
   return NULL;
}
