static bool cmd_wifi_dhcp(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_dns(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_ip(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_gateway(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_netmask(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_ntp(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_ap(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_sta(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_staent(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_syslog(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_ap_pass(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_ap_ssid(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_ap_timeout(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_sta_add_ap(Stream *ch, const char *args[]) {
   wifi_add_ap(args[0], args[1]);
}

static bool cmd_wifi_sta_del_ap(Stream *ch, const char *args[]) {
}

static bool cmd_wifi_sta_list_aps(Stream *ch, const char *args[]) {
}
