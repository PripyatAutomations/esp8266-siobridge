#if	!defined(_SIOBRIDGE_logger_h)
#define	_SIOBRIDGE_logger_h
#include <stdarg.h>
enum LogPriority {
      P_CRIT = 0,
      P_WARN,
      P_INFO,
      P_DEBUG,
};
typedef enum LogPriority logpriority_t;

extern void Log(logpriority_t priority, const char *fmt, ...);

/* syslog */
extern void logger_setup(void);

#endif
