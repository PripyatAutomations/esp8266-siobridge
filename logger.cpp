/*
 * support logging to a a few places
 *	Targets: syslog console flash
 */
#include "siobridge.h"
#include <EspSyslog/Syslog.h>
#include <stdarg.h>

WiFiUDP udp;
Syslog *logger = NULL;

void syslog_setup(void) {
   if (logger != NULL)
      return;

   logger = new Syslog(cfg.hostname, "siobridge", cfg.syslog_host);
}

