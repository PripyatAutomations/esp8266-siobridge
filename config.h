#if	!defined(_SIOBRIDGE_CONFIG_H)
#define	_SIOBRIDGE_CONFIG_H

/* This should be setup so that RX-only and TX-only ports can happen */
#define	MAX_PORTS	16
#define	MAX_USERS	10
#define	MAX_APS		4
#define	L_ADMIN_USER	24
#define	L_ADMIN_PASS	32
#define	L_WIFI_SSID	32
#define	L_WIFI_PASS	32

enum sio_parity { PARITY_NONE, PARITY_ODD, PARITY_EVEN };
typedef enum sio_parity sio_parity_t;

typedef struct sio_port sio_port_t;
struct sio_port {
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
   WiFiPhyMode_t wifi_type;
   float wifi_power;			/* wifi power in dBm */
   time_t wifi_timeout;
   char hostname[128];
   char syslog_host[128];		/* syslog server */
   char admin_user[L_ADMIN_USER];
   char admin_pass[L_ADMIN_PASS];
   sio_port_t ports[MAX_PORTS];
   /* show *'s instead of passwords in info? */
   bool redact_passwords;

   /* http */
   int http_port;

   /* WiFi Client mode */
   IPAddress wifi_sta_ip,
             wifi_sta_gw,
             wifi_sta_net;

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
   char ssid[L_WIFI_SSID];
   char pass[L_WIFI_PASS];
};

extern bool config_load(void);
extern const char parity_to_str(sio_parity_t p);
extern Config_t cfg;

#endif
