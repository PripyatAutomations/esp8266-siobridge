#if	!defined(_SIOBRIDGE_USERS_H)
#define	_SIOBRIDGE_USERS_H

class SIOuser {
    private:
       char user[16];
       char pass[32];
       int privilege;		/* privileges */

    public:
       int refcnt;		/* how many active logins for this user? */
       bool disabled;
       bool superuser;
       bool readonly;
       SIOuser(const char *user, const char *pass, int privilege);
       const char *GetUsername(void);
       bool Delete(void);
       const char *GetPrivs(void);
       const char *GetPass(void);
};

/* */
/* Users array */
extern SIOuser *users[MAX_USERS];
/* find a user name name string */
extern SIOuser *user_find(const char *user);
/* delete a user, if present */
extern bool user_delete(const char *name);
/* login a user and return the SIOuser or NULL */
extern SIOuser *user_login(Stream *ch, const char *user, const char *pass);
/* find a free spot in users array */
extern int find_users_slot(void);

#endif
