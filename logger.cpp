/*
 * support logging to a a few places
 *	Targets: syslog console flash
 */
#include "siobridge.h"
#include <stdarg.h>

Syslog *logger = NULL;

void syslog_setup(void) {
   if (logger != NULL)
      return;

   logger = new Syslog(cfg.hostname, "siobridge", cfg.syslog_host);
}

