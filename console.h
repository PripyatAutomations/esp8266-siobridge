#if	!defined(_SIOBRIDGE_console_h)
#define	_SIOBRIDGE_console_h

/* console */
typedef struct MenuItem MenuItem;
struct MenuItem {
   const char key;
   bool (*func)(Stream *ch, const char *args[]);
   const MenuItem *submenu;
};

typedef struct Menu Menu;
struct Menu {
   char name[16];
   MenuItem *menu;
   const char **help;
};

extern const char *s_connected;
extern const char *s_disconnected;
extern const char *s_redacted;
extern const char *sio_connected(int port);
extern unsigned int sio_baud(int port);
extern unsigned int sio_unread_buffers(int port);
extern const char *sio_bits(int port);
extern const char *console_prompt(Stream *ch, const char *prompt);
extern const char *redact_password(const char *p);
extern bool show_menu(Stream *ch, const char *menu);
extern bool show_help(Stream *ch, const char *menu);
extern const char *get_tty_from_stream(Stream *);

#endif
