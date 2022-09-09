#if	!defined(_SIOBRIDGE_USERS_H)
#define	_SIOBRIDGE_USERS_H

typedef struct user user_t;
struct user {
   char user[16];
   char pass[32];
};

extern user_t users[MAX_USERS];
extern user_t user_add(const char *user, const char *pass);
extern user_t *user_find(const char *user);
extern void user_del(const char *user);
extern bool user_authenticate(const char *user, const char *pass);

#endif
