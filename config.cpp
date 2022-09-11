/*
 * Our configuration parser...
 *
 * This loads /config.txt from the chosen filesystem and parses it into a Config_t
 */
#include "siobridge.h"
#include "cfg_general.h"
#include "cfg_ports.h"
#include "cfg_wifi.h"
#include "cfg_ports.h"

Config_t cfg;
AccessPoint aps[MAX_APS];

/* Parse a line against the config items array passed */
static bool config_parse_line(config_item_t *ci, const char *key, const char *val, int line) {
//   Serial.printf("config_parse_line: config.txt:%d: %s=%s\r\n", line, key, val);
   /* walk through the available configuration keys and try to find a match */
   for (int i = 0; ci[i].key[0] != '\0'; i++) {
//      Serial.printf("i: %d\r\n", i);

      if (strcasecmp(ci[i].key, key) == 0) {
//         Serial,printf("Match %i\r\n", i);

         switch (ci[i].valtype) {
            case T_NONE:
               Serial.printf("* wtf? config_item_t %s matches (%i) but valtype == T_NONE ;(\r\n", key, i);
               return false;
            case T_INT:
               *ci[i].ival = atoi(val);
#if	defined(SIO_DEBUG)
               Serial.printf("* Set <int> %s = %d\r\n", key, ci[i].ival);
#endif
               break;
            case T_CHAR:
               memset(ci[i].cval, 0, sizeof(ci[i].cval));
               strncpy(ci[i].cval, val, sizeof(&ci[i].cval));
#if	defined(SIO_DEBUG)
               Serial.printf("* Set <char> %s = %s\r\n", key, ci[i].cval);
#endif
               break;
            case T_BOOL:
               *ci[i].bval = parse_bool(val);
#if	defined(SIO_DEBUG)
               Serial.printf("* Set <bool> %s = %s\r\n", key, bool_to_str(ci[i].bval));
#endif
               break;
            case T_FUNC:
#if	defined(SIO_DEBUG)
               Serial.printf("* SetCall <func> %s = %s\r\n", key, val);
#endif
//               ci[i].func(key, val, line);
               break;
            case T_FLOAT:
               *ci[i].fval = (float)sscanf(val, "%f");
#if	defined(SIO_DEBUG)
               Serial.printf("* Set <float> %s = %f\r\n", key, ci[i].fval);
#endif
               break;
            default:
               Serial.printf("* wtf? unknown valtype at config.txt:%d key=%s type=%d (UNKNOWN)\r\n", line, key, ci[i].valtype);
               return false;
         }
         return true;
      }
   }
   Serial.printf("* config_parse_line: Unknown key %s at config.txt:%d\r\n", key, line);
   return false;
}

bool config_load(void) {
   char buf[128];
   unsigned int line = 0;
   bool eof = false;
   char section[32];
   bool in_comment = false;

#if	defined(USE_SPIFFS)
   File cf = SPIFFS.open("/config.txt", "r"); 
#endif
#if	defined(USE_LITTLEFS)
   File cf = LittleFS.open("/config.txt", "r"); 
#endif

   if (cf == NULL) {
      Serial.printf("* ERROR: config.txt missing\r\n");
      flash_dir();
      return false;
   }

   Serial.printf("* Parsing config.txt\r\n");

   memset(section, 0, sizeof(section));

   while (!eof) {
      char *vp = NULL;
      size_t rd = 0;		/* read length */
      char *bp = buf;

      /* zero the buffer */
      memset(buf, 0, sizeof(buf));

      /* count lines for debugging */
      line++;
      rd = cf.readBytesUntil('\n', buf, 128);

      /* did we read anything? */
      if (rd <= 0) {
         eof = true;
         Serial.printf("* End of config.txt\r\n");
         break;
      }

//      Serial.printf("Read |%s| %d bytes. Trimming leading spaces\r\n", bp, rd);
      /* eat leading spaces */
      while (*bp == ' ' || *bp == '\t')
          bp++;

//      Serial.printf("String |%s| is now %d bytes long. Trimming trailing junk\r\n", bp, strlen(bp));

      /* eat trailing spaces/line endings */
      char *ep = bp + strlen(bp) + 1;
      while(*ep == ' ' || *ep == '\t' || *ep == '\r' || *ep == '\n') {
         *ep = '\0';
         ep--;
      }

//      Serial.printf("String |%s| is now %d bytes long after all trimming\r\n", bp, strlen(bp));

      if (strlen(bp) == 0) {
         eof = true;
         break;
      }

      /* Skip single line comments */
      if (*bp == '#' || *bp == ';' || (*bp == '/' && *(bp+1) == '/'))
         continue;

      /* support multi-line comments */
      if (in_comment) {
         if (*bp == '*' && *(bp+1) == '/')
            in_comment = false;

         continue;
      }

      /* handle configuration sections */
      if (*bp == '[' &&  *(ep-2) == ']') {
         memset(section, 0, sizeof(section));
         strncpy(section, bp + 1, (ep - bp) - 3);
//         Serial.printf("Opening section %s\r\n", section);
         continue;
      }

      vp = strchr(bp, '=') + 1;
      /* Terminate the base string (replace = with NULL) */
      *(vp - 1) = '\0';

      if (strncasecmp(section, "general", 7) == 0) {
        config_parse_line(config_items_general, bp, vp, line);
      } else if (strncasecmp(section, "wifi", 4) == 0) {
        char *n = section + 4;
        int apn = atoi(n);

        /* Create a mutated config_items_t that points to THIS ap's properties */
        if (aps[apn].ci == NULL)
           aps[apn].ci = mutate_wifi_ci(config_items_wifi, apn);

        config_parse_line(aps[apn].ci, bp, vp, line);
      } else if (strncasecmp(section, "ports", 5) == 0) {
        char *n = section + 5;
        int pn = atoi(n);

        /* Create a mutated config_items_t that points to THIS ap's properties */
        if (ports[pn].ci == NULL)
           ports[pn].ci = mutate_ports_ci(config_items_ports, pn);
        config_parse_line(ports[pn].ci, bp, vp, line);
      } else if (strncasecmp(section, "users", 5) == 0) {
         user_add(bp, vp);
      } else {
         Serial.printf("\r\n* Invalid configuration section |%s| in config.txt:%s\r\n", section, line);
      }
   }
   Serial.printf("\r\n* Finished loading configuration\r\n");
}

static bool config_dump_section(Stream *ch, config_item_t *ci) {
   for (int i = 0; ci[i].key[0] != '\0'; i++) {
      switch(ci[i].valtype) {
         case T_INT:
            ch->printf("%s=%i\r\n", ci[i].key, ci[i].ival);
            break;
         case T_CHAR:
            ch->printf("%s=%s\r\n", ci[i].key, ci[i].cval);
            break;
         case T_BOOL:
            ch->printf("%s=%s\r\n", ci[i].key, bool_to_str(ci[i].bval));
         case T_FLOAT:
            ch->printf("%s=%f\r\n", ci[i].key, ci[i].fval);
            break;
         case T_FUNC:
            if (ci[i].dump_func != NULL) {
               ci[i].dump_func(ch);
            }
            break;
         case T_NONE:
         default:
            break;
      }
   }
   return false;
}

static bool config_dump_ports(Stream *ch) {
   return false;
}

static bool config_dump_wifis(Stream *ch) {
   return false;
}

/*
 * dump configuration as ASCII text to the desired stream (file, serial port, etc)
 */
bool config_dump(Stream *ch) {
   ch->printf("!config version %s\r\n\r\n", VERSION);
   ch->printf("[general]\r\n");
   config_dump_section(ch, config_items_general);
   /* Dump section: general */
   ch->printf("[ports]\r\n");
   config_dump_ports(ch);
   ch->printf("[wifi]\r\n");
   config_dump_wifis(ch);
   return true;
}
