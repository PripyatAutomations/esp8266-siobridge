#if	!defined(_SIOBRIDGE_telnet_h)
#define	_SIOBRIDGE_telnet_h

/* telnet/http services */
extern void telnet_loop(void), http_loop(void);
extern void telnet_setup(void), http_setup(void);
extern void telnet_stop(void), http_stop(void);

#endif
