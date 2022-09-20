/*
 * support for logins and multiuser
 */
#include "siobridge.h"
const char *s_privs_super = "ADMIN";
const char *s_privs_readonly = "readonly";
const char *s_privs_normal = "user";

SIOuser *users[MAX_USERS];

SIOuser::SIOuser(const char *user, const char *pass, int privilege) {
    if (user == NULL || pass == NULL)
       return;

    strncpy(this->user, user, sizeof(this->user));
    strncpy(this->pass, pass, sizeof(this->pass));

    Log(P_INFO, "* Added user %s", this->user);
}

const char *SIOuser::GetPrivs(void) {
   if (this->readonly)
      return s_privs_readonly;

   if (this->superuser)
      return s_privs_super;

   return s_privs_normal;
}

bool SIOuser::Delete(void) {
    Log(P_DEBUG, "* Deleting user %s", this->user);
    return true;
}

bool user_delete(const char *name) {
    SIOuser *u = user_find(name);
    if (u != NULL) {
       u->Delete();
       return true;
    } else
       Serial.printf("ERROR deleting user %s: Not Found!\r\n", name);

    return false;
}
/* These apply to the users list as a whole so belong outside the classes */
SIOuser *user_find(const char *user) {
    for (int i = 0; i < MAX_USERS; i++) {
       if (strcasecmp(users[i]->GetUsername(), user) == 0) {
          return users[i];
       }
    }
    Log(P_DEBUG, "* User lookup for %s failed", user);
    return NULL;
}

SIOuser *user_login(Stream *ch, const char *user, const char *pass) {
    SIOuser *u = user_find(user);

    if (u == NULL) {
       Serial.printf("login failed for %s\r\n", user);
       return NULL;
    }

    if (strcasecmp(u->GetPass(), pass) == 0) {
       Serial.printf("succesful login from %s (%d)\r\n", user, u->GetPrivs());
       /* XXX: Make a session */
       // Session *newsess = session_start(Stream *ch, SIOuser *user);
       ch->printf("You are logged in as %s with %s privileges.\r\n", u->GetUsername(), u->GetPrivs());
       Log(P_WARN, "Login from %s on %s", user, get_tty_from_stream(ch));
       return u;
    }
    return NULL;
}

int find_users_slot(void) {
    int i = 0;

    for (i = 0; users[i]->GetUsername() != NULL; i++)
       ;

    if (i < MAX_USERS)
       return i;

    return -1;
}

const char *SIOuser::GetUsername(void) {
   return this->user;
}

const char *SIOuser::GetPass(void) {
   return this->pass;
}
