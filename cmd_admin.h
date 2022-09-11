#if	!defined(_SIOBRIDGE_cmd_admin_h)
#define	_SIOBRIDGE_cmd_admin_h

/*
 * cmd_info: System information display
 */
static bool cmd_info(Stream *ch, const char *args[]) {
   ch->printf("**************************\r\n");
   ch->printf("* esp8266-siobridge info *\r\n");
   ch->printf("**************************\r\n");
   ch->printf("\r\n");
   ch->printf("Serial Ports:\r\n");

   for (int i = 0; i < (MAX_PORTS - 1); i++)
      ch->printf("\tsio%d: %s <%s@%d>\n", i, sio_connected(i), sio_unread_buffers(i), sio_baud(i));

   ch->printf("\r\n");
   ch->printf("WiFi Status:\r\n");
   if (cfg.wifi_mode == WIFI_AP || cfg.wifi_mode == WIFI_AP_STA)
      ch->printf("\tAP - SSID: \"%s\" PASS: \"%s\"\r\n", cfg.wifi_ap_ssid, redact_password(cfg.wifi_ap_pass));
//   if (cfg.wifi_mode == WIFI_STA || cfg.wifi_mode == WIFI_AP_STA)
//   ch->printf("\tSTA - SSID: \"%s\" PASS: \"%s\"\r\n", cfg.wifi_sta_ssid, redact_password(cfg.wifi_sta_pass));
   /* XXX: print client networks */
}


/*
 * cmd_restart: Restart the device
 */
static bool cmd_restart(Stream *ch, const char *args[]) {
   ch->println("* Restarting!");
   ESP.restart();
}

#endif
