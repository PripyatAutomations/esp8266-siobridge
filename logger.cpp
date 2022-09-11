/*
 * support logging to a a few places
 *	Targets: syslog console flash
 */
#include "siobridge.h"
#include <EspSyslog/Syslog.h>
#include <stdarg.h>

WiFiUDP udp;
Syslog *logger = NULL;
FILE   *logfp = NULL;

void logger_setup(void) {
   if (logger != NULL)
      return;

   logger = new Syslog(cfg.hostname, "siobridge", cfg.syslog_host);
}


void Log(logpriority_t priority, const char *fmt, ...) {
   char msgbuf[512];
   char tsbuf[128];
   va_list ap;

   /* clear the message buffer */
   memset(msgbuf, 0, sizeof(msgbuf));

   va_start(ap, fmt);

   /* Expand the format string */
   vsnprintf(msgbuf, 511, fmt, ap);

   /* create timestamp string for file logs */
   if (logfp != NULL) {
      time_t t;
      struct tm *tmp;

      memset(tsbuf, 0, sizeof(tsbuf));
      t = time(NULL);

      if ((tmp = localtime(&t)) != NULL) {
         /* success, proceed */
         if (strftime(tsbuf, sizeof(tsbuf), "%Y/%m/%d %H:%M:%S", tmp) == 0) {
            /* handle the error */
            memset(tsbuf, 0, sizeof(tsbuf));
            snprintf(tsbuf, sizeof(tsbuf), "unknown");
         }
      }
   }

   /* Route the message according to priority */
   switch (priority) {
      case P_CRIT:
         if (logger != NULL)
            logger->error(msgbuf);
         if (logfp != NULL) {
            fprintf(logfp, "[%s] CRIT: %s\n", tsbuf, msgbuf);
            fflush(logfp);
         }
         break;
      case P_WARN:
         if (logger != NULL)
            logger->warn(msgbuf);
         if (logfp != NULL) {
            fprintf(logfp, "[%s] WARN: %s\n", tsbuf, msgbuf);
            fflush(logfp);
         }
         break;
      case P_INFO:
         if (logger != NULL)
            logger->info(msgbuf);
         if (logfp != NULL) {
            fprintf(logfp, "[%s] INFO: %s\n", tsbuf, msgbuf);
            fflush(logfp);
         }
         break;
      case P_DEBUG:
         if (logger != NULL)
            logger->debug(msgbuf);
         if (logfp != NULL) {
            fprintf(logfp, "[%s] DEBUG: %s\n", tsbuf, msgbuf);
            fflush(logfp);
         }
         break;
      default:
         break;
   }

   va_end(ap);
}
