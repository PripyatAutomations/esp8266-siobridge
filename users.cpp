/*
 * support for logins and multiuser
 */
#include "siobridge.h"

user_t users[MAX_USERS];

user_t *user_add(const char *user, const char *pass) {
    user_t *p = (user_t *)malloc(sizeof(user_t));

    if (user == NULL || pass == NULL)
       return NULL;

    if (!p) {
       Serial.printf("OOM in user_add! :(\r\n");
       delay(500);
       ESP.restart();
    }

    strncpy(p->user, user, sizeof(p->user));
    strncpy(p->pass, pass, sizeof(p->pass));

    return p;
}

user_t *user_find(const char *user) {
    for (int i = 0; i < MAX_USERS; i++) {
       if (strcasecmp(users[i].user, user) == 0) {
          return &users[i];
       }
    }
    return NULL;
}

bool user_delete(const char *user) {
    user_t *p = user_find(user);

    if (p == NULL) {
       Serial.printf("user_del(%s): Unknown user.\r\n", user);
       return false;
    }

    /* erase the user entry */
    memset(p, 0, sizeof(user_t));
    return true;
}

bool user_authenticate(const char *user, const char *pass) {
    user_t *u = user_find(user);

    if (u == NULL) {
       Serial.printf("login failed for %s\r\n", user);
       return false;
    }

    if (strcasecmp(u->pass, pass) == 0) {
       Serial.printf("succesful login from %s\r\n", user);
       return true;
    }
    return false;
}
