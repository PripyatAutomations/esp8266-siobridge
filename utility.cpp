#include "siobridge.h"

/* String constants we use more than a few times */
const char *s_connected = "connected";
const char *s_disconnected = "disconencted";
const char *s_redacted = "*****";
const char *s_false = "false";
const char *s_true = "true";
const char *s_none = "NONE";
const char *s_prio_crit = "CRIT",
           *s_prio_warn = "WARN",
           *s_prio_info = "INFO",
           *s_prio_debug = "DEBUG";

const char *s_8n1 = "8N1", *s_8n2 = "8N2", *s_8e1 = "8E1", *s_8e2 = "8E2", *s_8o1 = "8O1", *s_8o2 = "8O2",
           *s_7n1 = "7N1", *s_7n2 = "7N2", *s_7e1 = "8E1", *s_7e2 = "7E2", *s_7o1 = "7O1", *s_7o2 = "7O2";

bool is_ip(String str) {
   for (size_t i = 0; i < str.length(); i++) {
      int c = str.charAt(i);

      if (c != '.' && (c < '0' || c > '9')) {
         return false;
      }
   }
   return true;
}

String ip_to_string(IPAddress ip) {
   String res = "";
   for (int i = 0; i < 3; i++) {
      res += String((ip >> (8 * i)) & 0xFF) + ".";
   }
   res += String(((ip >> 8 * 3)) & 0xFF);
   return res;
}


const char *sio_connected(int port) {
   if (cfg.ports[port].refcnt > 0)
      return s_connected;

   return s_disconnected;
}


unsigned int sio_baud(int port) {
   return cfg.ports[port].baud_rate;
}

unsigned int sio_unread_buffers(int port) {
   return cfg.ports[port].unread_buffers;
}

const char *sio_mode_str(int port) {
   sio_port_t *p = &cfg.ports[port];

   if (port > MAX_PORTS - 1)
      return NULL;

   if (p->data_bits == 8) {
      if (p->parity == PARITY_NONE) {
          if (p->stop_bits == 1) {
            return s_8n1;
         } else if (p->stop_bits == 2) {
            return s_8n2;
         }
      } else if (p->parity == PARITY_EVEN) {
         if (p->stop_bits == 1) {
            return s_8e1;
         } else if (p->stop_bits == 2) {
            return s_8e2;
         }
      } else if (p->parity == PARITY_ODD) {
           if (p->stop_bits == 1) {
            return s_8o1;
         } else if (p->stop_bits == 2) {
            return s_8o2;
         }
      }
   } else if (p->data_bits == 7) {
      if (p->parity == PARITY_NONE) {
          if (p->stop_bits == 1) {
            return s_7n1;
         } else if (p->stop_bits == 2) {
            return s_7n2;
         }
      } else if (p->parity == PARITY_EVEN) {
         if (p->stop_bits == 1) {
            return s_7e1;
         } else if (p->stop_bits == 2) {
            return s_7e2;
         }
      } else if (p->parity == PARITY_ODD) {
           if (p->stop_bits == 1) {
            return s_7o1;
         } else if (p->stop_bits == 2) {
            return s_7o2;
         }
      }
   }
   return s_none;
}


/* Simple mechanism to avoid showing passwords on the console, if needed */
const char *redact_password(const char *p) {
   if (cfg.redact_passwords)
      return s_redacted;

   return p;
}

bool parse_bool(const char *str) {
   if (strncasecmp(str, "true", 4) == 0 ||
       strncasecmp(str, "yes", 3) == 0 ||
       strncasecmp(str, "on", 2) == 0) {
      return true;
   }
   if (strncasecmp(str, "false", 5) == 0 ||
       strncasecmp(str, "off", 3) == 0 ||
       strncasecmp(str, "no", 2) == 0) {
      return false;
   }
   return false;
}

const char parity_to_str(sio_parity_t p) {
   if (p == PARITY_NONE)
      return 'N';
   else if (p == PARITY_EVEN)
      return 'E';
   else if (p == PARITY_ODD)
      return 'O';
}

const char *bool_to_str(bool b) {
   if (b == true)
      return s_true;
   else
      return s_false;
}

const char *log_priority_to_str(logpriority_t priority) {
   switch (priority) {
      case P_CRIT:
         return s_prio_crit;
         break;
      case P_WARN:
         return s_prio_warn;
         break;
      case P_INFO:
         return s_prio_info;
         break;
      case P_DEBUG:
         return s_prio_info;
         break;
      default:
         return s_none;
         break;
   }
   return NULL;
}
