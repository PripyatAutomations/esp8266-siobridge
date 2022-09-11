/*
 * support logging to a a few places
 *	Targets: syslog console flash (file)
 */
#include "siobridge.h"
#include <EspSyslog/Syslog.h>
#include <stdarg.h>

Syslog *logger = NULL;
FILE   *logfp = NULL;

void logger_setup(void) {
   if (logger == NULL)
      logger = new Syslog(cfg.hostname, "siobridge", cfg.syslog_host);

#if	defined(USE_LOGFILE)
   if (logfp == NULL) {
#if	defined(USE_SPIFFS)
      SPIFFS.open(LOGFILE, "w");
#endif
#if	defined(USE_LITTLEFS)
      LittleFS.open(LOGFILE, "w");
#endif
   }
#endif
}

void logger_end(void) {
   if (logfp != NULL)
      fclose(logfp);
}

void Log(logpriority_t priority, const char *fmt, ...) {
   char msgbuf[512];
   char tsbuf[64];
   va_list ap;
   time_t t;
   struct tm *tmp;

   /* clear the message buffer */
   memset(msgbuf, 0, sizeof(msgbuf));

   va_start(ap, fmt);

   /* Expand the format string */
   vsnprintf(msgbuf, 511, fmt, ap);

   memset(tsbuf, 0, sizeof(tsbuf));
   t = time(NULL);

   if ((tmp = localtime(&t)) != NULL) {
      /* success, proceed */
      if (strftime(tsbuf, sizeof(tsbuf), "%Y/%m/%d %H:%M:%S", tmp) == 0) {
         /* handle the error */
         memset(tsbuf, 0, sizeof(tsbuf));
         snprintf(tsbuf, sizeof(tsbuf), "unknown<%lu>", time(NULL));
      }
   }

   /* Only spew to the serial port if syslog and logfile are closed */
   if (logger == NULL && logfp == NULL) {
      Serial.printf("[%s] %s: %s\n", log_priority_to_str(priority), tsbuf, msgbuf);
      va_end(ap);
      return;
   }

   /* log to the logfile on flash */
   if (logfp != NULL) {
      fprintf(logfp, "[%s] %s: %s\n", tsbuf, log_priority_to_str(priority), msgbuf);
      fflush(logfp);
   }

   /* Route the message according to priority */
   switch (priority) {
      case P_CRIT:
         if (logger != NULL)
            logger->error(msgbuf);
         break;
      case P_WARN:
         if (logger != NULL)
            logger->warn(msgbuf);
         break;
      case P_INFO:
         if (logger != NULL)
            logger->info(msgbuf);
         break;
      case P_DEBUG:
         if (logger != NULL)
            logger->debug(msgbuf);
      default:
         break;
   }

   va_end(ap);
}
