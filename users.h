#if	!defined(_SIOBRIDGE_USERS_H)
#define	_SIOBRIDGE_USERS_H

typedef struct user user_t;
struct user {
   bool disabled;
   bool superuser;
   bool readonly;
   char user[16];
   char pass[32];
   int refcnt;		/* how many active logins for this user? */
};

extern user_t users[MAX_USERS];
extern user_t *user_add(const char *user, const char *pass);
extern user_t *user_find(const char *user);
extern bool user_delete(const char *user);
extern bool user_authenticate(Stream *ch, const char *user, const char *pass);
extern const char *privilege_str(user_t *u);

#endif
