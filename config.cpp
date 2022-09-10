/*
 * Our configuration parser...
 *
 * This loads /config.txt from the chosen filesystem and parses it into a Config_t
 */
#include "siobridge.h"

Config_t cfg;

AccessPoint aps[MAX_APS];

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

bool config_load(void) {
   char buf[128];
   int line = 0;
   bool eof = false;
   char section[32];
   bool in_comment = false;

#if	defined(USE_SPIFFS)
   File cf = SPIFFS.open("/config.txt", "r"); 
#endif
#if	defined(USE_LITTLEFS)
   File cf = LittleFS.open("/config.txt", "r"); 
#endif

   if (!cf) {
      Serial.printf("ERROR: config.txt missing\r\n");
      flash_dir();
      return false;
   }

   Serial.printf("* Parsing config.txt\r\n");

   memset(section, 0, sizeof(section));

   while (!eof) {
      char *vp = NULL;
      size_t rd = 0;		/* read length */
      int ap = 0;		/* selected AP# in [ap#] sections */
      char *bp = buf;

      /* zero the buffer */
      memset(buf, 0, sizeof(buf));

      /* count lines for debugging */
      line++;
      rd = cf.readBytesUntil('\n', buf, 128);

      /* did we read anything? */
      if (rd <= 0) {
         eof = true;
         break;
      }

      /* eat leading spaces */
      do {
         bp++;
      } while(*bp == ' ' || *bp == '\t');

      /* Skip single line comments */
      if (*bp == '#' || *bp == ';' ||
          (*bp == '/' && *(bp+1) == '/'))
         continue;

      /* support multi-line comments */
      if (in_comment) {
         if (*bp == '*' && *(bp+1) == '/')
            in_comment = false;

         continue;
      }

      /* handle configuration sections */
      if (*bp == '[' && buf[sizeof(buf)-1] == ']') {
         memset(section, 0, sizeof(section));
         strncpy(section, bp+1, strlen(bp) - 2);

         if (strncasecmp(section, "ap", 2) == 0) {
            /* parse out the ap # we are working on */
            char *apn = section + 2;
            /* Default to 0 if no # specified */
            if (*apn == ']')
               ap = 0;
            else
               ap = atoi(apn);
            Serial.printf("Adding AP#%d:\n", ap);
         }
      }
      vp = strchr(bp, '=') + 1;

      if (strncasecmp(section, "general", 7) == 0) {
         if (strncasecmp(buf, "wifi_mode", 9) == 0) {
           if (strncasecmp(vp, "ap", 2) == 0) {
              cfg.wifi_mode = WIFI_AP;
           } else if (strncasecmp(vp, "sta", 3) == 0) {
              cfg.wifi_mode = WIFI_STA;
           } else if (strncasecmp(vp, "both", 4) == 0) {
              cfg.wifi_mode = WIFI_AP_STA;
           } else
              Serial.printf("malformed line: config.txt:%d", line);
         } else if (strncasecmp(buf, "http_port", 9) == 0) {
           cfg.http_port = atoi(vp);
           Serial.printf("* Set http port to %s\r\n", cfg.http_port);
         } else if (strncasecmp(buf, "hostname", 8) == 0) {
           strncpy(cfg.hostname, vp, sizeof(cfg.hostname));
           Serial.printf("* Set hostname to %s\r\n", cfg.hostname);
         } else if (strncasecmp(buf, "connect", 7) == 0) {
           char ssid[L_WIFI_SSID+1];
           char pass[L_WIFI_PASS+1];
           char *sep = strchr(vp, '+');
           size_t s_len = 0, p_len = 0;

           if (sep != NULL) {
              s_len = (size_t)(sep - vp);
              if (s_len <= 0) {
                 Serial.printf("malformed line: config.txt:%d\r\n", line);
                 continue;
              }
           }
           memcpy(ssid, vp, s_len);
           ssid[s_len] = '\0';
           p_len = strlen(vp) - s_len;
           memcpy(pass, (vp + s_len), strlen(vp) - s_len);
           pass[p_len] = '\0';
           /* Add the AP to our list */
           wifi_add_ap(ssid, pass);
           /* XXX: Store it somewhere we can save it back to flash later */
         } else if (strncasecmp(buf, "wifi_timeout", 12) == 0) {
           cfg.wifi_timeout = atoll(vp);
         } else if (strncasecmp(buf, "wifi_mac", 8) == 0) {
   //        strncpy(cfg.wifi_mac, 
         } else if (strncasecmp(buf, "wifi_power", 10) == 0) {
           cfg.wifi_power = atof(vp);
         } else if (strncasecmp(buf, "wifi_type", 9) == 0) {
           switch(*vp) {
              case 'B':
              case 'b':
                cfg.wifi_type = WIFI_PHY_MODE_11B;
                break;
              case 'G':
              case 'g':
                cfg.wifi_type = WIFI_PHY_MODE_11G;
                break;
              case 'N':
              case 'n':
                cfg.wifi_type = WIFI_PHY_MODE_11N;
              break;
           }
         } else if (strncasecmp(buf, "ap_pass", 7) == 0) {
           strncpy(cfg.wifi_ap_pass, vp, L_WIFI_PASS);
         } else if (strncasecmp(buf, "ap_ssid", 7) == 0) {
           strncpy(cfg.wifi_ap_ssid, vp, L_WIFI_SSID);
         } else if (strncasecmp(buf, "ap_chan", 7) == 0) {
           cfg.wifi_ap_chan = atoi(vp);
         } else if (strncasecmp(buf, "ap_hidden", 9) == 0) {
           if (strncasecmp(vp, "yes", 3) == 0) {
             cfg.wifi_ap_hidden = true;
           } else if (strncasecmp(vp, "no", 2) == 0) {
             cfg.wifi_ap_hidden = false;
           } else {
             Serial.printf("Invalid value on config.txt:%d\r\n", line);
           }
         } else if (strncasecmp(buf, "ap_wait", 7) == 0) {
           cfg.wifi_ap_wait = atoi(vp);
         } else if (strncasecmp(buf, "admin_user", 11) == 0) {
           strncpy(cfg.admin_user, vp, L_ADMIN_USER);
         } else if (strncasecmp(buf, "admin_pass", 11) == 0) {
           strncpy(cfg.admin_pass, vp, L_ADMIN_PASS);
         } else if (strncasecmp(buf, "redact_passwords", 16) == 0) {
           if (strncasecmp(vp, "yes", 3) == 0) {
             cfg.redact_passwords = true;
           } else if (strncasecmp(vp, "no", 2) == 0) {
             cfg.redact_passwords = false;
           } else {
             Serial.printf("Invalid value on config.txt:%d\r\n", line);
           }
         } else if (strncasecmp(buf, "sio", 3) == 0) {
            int port = atoi(buf+4);
            char *dp = strchr(buf, '.');
            char *pin = dp + 3;
            sio_port_t *sp = NULL;

            /* detect errors */
            if (port < 0 || port > MAX_PORTS || (dp == NULL)) {
               Serial.printf("Invalid line at config:%d\r\n", line);
               continue;
            }

            /* sio port selected */
            sp = &cfg.ports[port];

            /* This is a TX entry */
            if (tolower((*dp+1)) == 't') {
               sp->tx_pin = atoi(vp);
            } else if (tolower((*dp+1)) == 't') { /* or is it a RX pin? */
               sp->rx_pin = atoi(vp);
            } else if (strncasecmp(dp+1, "telnet", 6) == 0) {
               sp->telnet_port = atoi(vp);
            } else if (strncasecmp(dp+1, "console", 7) == 0) {
               if (strncasecmp(vp, "yes", 3) == 0) {
                 sp->console = true;
               } else if (strncasecmp(vp, "no", 2) == 0) {
                 sp->console = false;
               } else {
                 Serial.printf("invalid value %s in config.txt:%d\r\n", vp, line);
               }
            } else if (strncasecmp(dp+1, "trusted", 7) == 0) {
               if (strncasecmp(vp, "yes", 3) == 0) {
                 sp->trusted = true;
                } else if (strncasecmp(vp, "no", 2) == 0) {
                 sp->trusted = false;
               } else {
                Serial.printf("invalid value %s in config.txt:%d\r\n", vp, line);
               }
            } else if (strncasecmp(dp+1, "mode", 4) == 0) {
              // Ahh it's the mode...
              /* Figure out the data bits */
              switch (*vp) {
                 case '8':
                    sp->data_bits = 8;
                    break;
                 case '7':
                    sp->data_bits = 7;
                    break;
                 default:
                    Serial.printf("Invalid mode %s at config.txt:%d\r\n", vp, line);
                    break;
              }

              /* parity */
              switch (*vp+1) {
                 case 'N':
                 case 'n':
                    break;
                 case 'E':
                 case 'e':
                    break;
                 case 'O':
                 case 'o':
                    break;
                 default:
                    Serial.printf("Invalid mode %s at config.txt:%d\r\n", vp, line);
                    break;
              }
              /* stop bits */
              switch (*vp+2) {
                 case '2':
                    sp->stop_bits = 2;
                    break;
                 case '1':
                    sp->stop_bits = 1;
                    break;
                 default:
                    Serial.printf("Invalid mode %s at config.txt:%d\r\n", vp, line);
                    break;
              }
              /* baud rate */
              char *baudp = vp + 4;
              unsigned int baudrate = atoi(baudp);
              sp->baud_rate = baudrate;

   #if	defined(SIO_DEBUG)
              /* print some debug info */
              Serial.printf("* sio%d: tx=%d,rx=%d %d%c%d@%d configured\r\n",
                            port, sp->tx_pin, sp->rx_pin,
                            sp->data_bits, parity_to_str(sp->parity),
                            sp->stop_bits, sp->baud_rate);
   #endif
            }
         }
      } else if (strncasecmp(section, "ap", 2) == 0) {
         if (strncasecmp(buf, "enabled", 7) == 0) {
            aps[ap].enabled = parse_bool(vp);
         } else if (strncasecmp(buf, "ssid", 4) == 0) {
           /* aps[ap].ssid */
         } else if (strncasecmp(buf, "pass", 4) == 0) {
           /* aps[ap].pass */
         } else if (strncasecmp(buf, "dhcp", 4) == 0) {
           /* aps[ap].dhcp */
         } else if (strncasecmp(buf, "ip", 2) == 0) {
           /* aps[ap].ip */
         } else if (strncasecmp(buf, "gw", 2) == 0) {
           /* aps[ap].gw */
         } else if (strncasecmp(buf, "mask", 4) == 0) {
           /* aps[ap].mask */
         } else if (strncasecmp(buf, "dns", 3) == 0) {
         } else if (strncasecmp(buf, "ntp", 3) == 0) {
         }
      } else {
         Serial.printf("\r\n Invalid configuration section %s in config.txt:5d\r\n", section, line);
      }
   }
   Serial.printf("\r\n* Finished loading configuration\r\n");
}

const char parity_to_str(sio_parity_t p) {
   if (p == PARITY_NONE)
      return 'N';
   else if (p == PARITY_EVEN)
      return 'E';
   else if (p == PARITY_ODD)
      return 'O';
}

/* XXX: Write this */
bool config_dump(const char *name) {
#if	defined(USE_SPIFFS)
#endif
#if	defined(USE_LITTLEFS)
#endif

   /* wifi mode */
   switch(cfg.wifi_mode) {
      case WIFI_AP:
         break;
      case WIFI_STA:
         break;
      case WIFI_AP_STA:
         break;
      default:
         break;
   }
   return true;
}
