#if	!defined(_SIOBRIDGE_utility_h)
#define	_SIOBRIDGE_utility_h

#define	STRINGIFY(x)	#x

/* constant strings */
extern const char *s_privs_super,
                  *s_privs_readonly,
         	  *s_privs_normal,
         	  *s_connected,
        	  *s_disconnected,
         	  *s_redacted,
         	  *s_false,
         	  *s_true,
         	  *s_none;

extern const char *s_8n1, *s_8n2, *s_8e1, *s_8e2, *s_8o1, *s_8o2,
           *s_7n1, *s_7n2, *s_7e1, *s_7e2, *s_7o1, *s_7o2;
extern const char *s_prio_crit, *s_prio_warn, *s_prio_info, *s_prio_debug;

/* functions */
extern bool is_ip(String str);
extern String ip_to_string(IPAddress ip);
extern const char *sio_connected(int port);
extern unsigned int sio_baud(int port);
extern unsigned int sio_unread_buffers(int port);
extern const char *sio_mode_str(int port);
extern const char *redact_password(const char *p);
extern bool parse_bool(const char *str);
extern const char parity_to_str(sio_parity_t p);
extern const char *bool_to_str(bool b);
const char *log_priority_to_str(logpriority_t priority);

#endif
