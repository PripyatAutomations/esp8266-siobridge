#if	!defined(_SIOBRIDGE_CONFIG_H)
#define	_SIOBRIDGE_CONFIG_H

/* Enable debugging? */
#define	SIO_DEBUG

/* Use either SPIFFS or LittleFS, not both! */
#define	USE_SPIFFS
//#define USE_LITTLEFS
//#define USE_MDNS
//#define USE_CAPTIVE_PORTAL

/*********************
 * Failsafe Settings *
 *********************/
/* Wait up to 300 seconds (5 minutes) in the WiFi config (AP) mode before restarting */
#define AP_CONFIG_TIMEOUT 300
#define AP_SSID   "REPEATER ADMIN"
#define	AP_PASS	  ""
#define	AP_CHAN	  1
#define	AP_HIDDEN false
#define ADMIN_USER "admin"
#define ADMIN_PASS "admin"

/* This should be setup so that RX-only and TX-only ports can happen */
#define	MAX_PORTS	16		/* maximum configured ports */
#define	MAX_USERS	5		/* maximum configured users */
#define	MAX_APS		4		/* maximum APs for WiFiMulti */
#define	MAX_SESSIONS	5		/* maximum concurrent sessions */
#define	L_ADMIN_USER	24
#define	L_ADMIN_PASS	32
#define	L_WIFI_SSID	32
#define	L_WIFI_PASS	32

enum sio_parity { PARITY_NONE, PARITY_ODD, PARITY_EVEN };
typedef enum sio_parity sio_parity_t;

typedef struct sio_port sio_port_t;
struct sio_port {
   bool configured, enabled;
   unsigned int tx_pin, rx_pin;
   unsigned int dtr_pin, cts_pin, rts_pin;
   int telnet_port;
   int refcnt;			/* connected users */
   int unread_buffers;		/* saved data waiting to be read? */

   /* 8N1, 7E1, etc */
   unsigned int baud_rate;
   sio_parity_t parity;
   unsigned int data_bits;
   unsigned int stop_bits;

   /* can use console? */
   bool console;
   /* can use admin commands? */
   bool trusted;
};

typedef struct Config Config_t;
struct Config {
   WiFiMode wifi_mode;
   WiFiPhyMode_t wifi_phymode;
   float wifi_power;			/* wifi power in dBm */
   int wifi_timeout;
   char admin_user[L_ADMIN_USER];
   char admin_pass[L_ADMIN_PASS];
   char wifi_mac[13];
   char hostname[128];
   char syslog_host[128];		/* syslog server(s) */
   char ntp_host[128];			/* time server(s) */
   char dns_host[128];			/* dns server(s) */
   sio_port_t ports[MAX_PORTS];

   /* show *'s instead of passwords in info? */
   bool redact_passwords;

   /* http */
   int http_port;

   /* WiFi Access Point mode */
   bool wifi_ap_hidden;
   char wifi_ap_ssid[L_WIFI_SSID];
   char wifi_ap_pass[L_WIFI_PASS];
   int wifi_ap_wait;
   int wifi_ap_chan;
   IPAddress wifi_ap_ip,
             wifi_ap_gw,
             wifi_ap_net;
};

typedef struct AccessPoint AccessPoint;
struct AccessPoint {
   bool dhcp;
   bool enabled;
   IPAddress ip, gw, mask;
   IPAddress dns1, dns2, dns3;
   char ssid[L_WIFI_SSID];
   char pass[L_WIFI_PASS];
};

typedef struct config_item config_item_t;
enum config_item_type { T_NONE = 0, T_CHAR, T_INT, T_BOOL, T_FLOAT, T_FUNC };

struct config_item {
   char key[24];
   enum config_item_type valtype;
   bool (*func)(const char *key, const char *val, int line);
   bool (*dump_func)(Stream *ch);
   /* */
   int *ival;
   char *cval;
   bool *bval;
   float *fval;
};

extern bool config_load(void);
extern const char parity_to_str(sio_parity_t p);
extern Config_t cfg;

#endif
