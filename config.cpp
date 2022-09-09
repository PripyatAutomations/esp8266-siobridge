/*
 * Our configuration parser...
 *
 * This loads /config.txt from the chosen filesystem and parses it into a Config_t
 */
#include "siobridge.h"

Config_t cfg;

bool config_load(void) {
   char buf[128];
   int line = 0;
   bool eof = false;
#if	defined(USE_SPIFFS)
   File cf = SPIFFS.open("/config.txt", "r"); 
#endif
#if	defined(USE_LITTLEFS)
   File cf = LittleFS.open("/config.txt", "r"); 
#endif

   if (!cf) {
      Serial.println("ERROR: config.txt missing");
      flash_dir();
      return false;
   }

   Serial.println("* Parsing config.txt");

   while (!eof) {
      /* read length */
      size_t rd = 0;
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

      char *vp = strchr(buf, '=') + 1;

      if (strncasecmp(buf, "wifi_mode", 9) == 0) {
        if (strncasecmp(vp, "ap", 2) == 0) {
           cfg.wifi_mode = WIFI_AP;
        } else if (strncasecmp(vp, "sta", 3) == 0) {
           cfg.wifi_mode = WIFI_STA;
        } else if (strncasecmp(vp, "auto", 4) == 0) {
           cfg.wifi_mode = WIFI_AP_STA;
        } else
           Serial.printf("malformed line: config.txt:%d", line);
      } else if (strncasecmp(buf, "connect", 7) == 0) {
        char ssid[L_WIFI_SSID+1];
        char pass[L_WIFI_PASS+1];
        char *sep = strchr(vp, '+');
        size_t s_len = 0, p_len = 0;

        if (sep != NULL) {
           s_len = (size_t)(sep - vp);
           if (s_len <= 0) {
              Serial.printf("malformed line: config.txt:%d\n", line);
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
      } else if (strncasecmp(buf, "ap_ssid", 7) == 0) {
        strncpy(cfg.wifi_ap_ssid, vp, L_WIFI_SSID);
      } else if (strncasecmp(buf, "ap_pass", 7) == 0) {
        strncpy(cfg.wifi_ap_pass, vp, L_WIFI_PASS);
      } else if (strncasecmp(buf, "ap_wait", 7) == 0) {
        cfg.wifi_ap_wait = atoi(vp);
      } else if (strncasecmp(buf, "admin_user", 11) == 0) {
        strncpy(cfg.admin_user, vp, L_ADMIN_USER);
      } else if (strncasecmp(buf, "admin_pass", 11) == 0) {
        strncpy(cfg.admin_pass, vp, L_ADMIN_PASS);
      } else if (strncasecmp(buf, "sio", 3) == 0) {
         int port = atoi(buf+4);
         char *dp = strchr(buf, '.');
         char *pin = dp + 3;
         sio_port_t *sp = NULL;

         /* detect errors */
         if (port < 0 || port > MAX_PORTS || (dp == NULL)) {
            Serial.printf("Invalid line at config:%d\n", line);
            continue;
         }

         /* sio port selected */
         sp = &cfg.ports[port];

         /* This is a TX entry */
         if (tolower((*dp+1)) == 't') {
            sp->tx_pin = atoi(vp);
         } else if (tolower((*dp+1)) == 't') { /* or is it a RX pin? */
            sp->rx_pin = atoi(vp);
         } else if (strncasecmp(dp+1, "console", 7) == 0) {
            if (strncasecmp(vp, "yes", 3) == 0) {
              sp->console = true;
            } else if (strncasecmp(vp, "no", 2) == 0) {
              sp->console = false;
            } else {
              Serial.printf("invalid value %s in config.txt:%d\n", vp, line);
            }
         } else if (strncasecmp(dp+1, "trusted", 7) == 0) {
            if (strncasecmp(vp, "yes", 3) == 0) {
              sp->trusted = true;
             } else if (strncasecmp(vp, "no", 2) == 0) {
              sp->trusted = false;
            } else {
             Serial.printf("invalid value %s in config.txt:%d\n", vp, line);
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
                 Serial.printf("Invalid mode %s at config.txt:%d\n", vp, line);
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
                 Serial.printf("Invalid mode %s at config.txt:%d\n", vp, line);
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
                 Serial.printf("Invalid mode %s at config.txt:%d\n", vp, line);
                 break;
           }
           /* baud rate */
           char *baudp = vp + 4;
           unsigned int baudrate = atoi(baudp);
           sp->baud_rate = baudrate;

#if	defined(SIO_DEBUG)
           /* print some debug info */
           Serial.printf("* sio%d: tx=%d,rx=%d %d%c%d@%d configured\n",
                         port, sp->tx_pin, sp->rx_pin,
                         sp->data_bits, parity_to_str(sp->parity),
                         sp->stop_bits, sp->baud_rate);
#endif
         }
      }
   }
   Serial.println("* Finished loading configuration");
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
