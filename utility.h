#if	!defined(_SIOBRIDGE_utility_h)
#define	_SIOBRIDGE_utility_h

#define	STRINGIFY(x)	#x

extern bool is_ip(String str);
extern String ip_to_string(IPAddress ip);
extern const char *sio_connected(int port);
extern unsigned int sio_baud(int port);
extern unsigned int sio_unread_buffers(int port);
extern const char *sio_mode_str(int port);
extern const char *redact_password(const char *p);
extern bool parse_bool(const char *str);
extern const char parity_to_str(sio_parity_t p);
extern const char *privilege_str(user_t *u);

#endif
