#if	!defined(_SIOBRIDGE_CONFIG_H)
#define	_SIOBRIDGE_CONFIG_H

/* This should be setup so that RX-only and TX-only ports can happen */
#define	MAX_PORTS	12
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
   unsigned int baud_rate;
   /* 8N1, 7E1, etc */
   sio_parity_t parity;
   unsigned int data_bits;
   unsigned int stop_bits;
   /* controls */
   /* can use console? */
   bool console;
   /* can use admin commands? */
   bool trusted;
};

typedef struct Config Config_t;
struct Config {
   WiFiMode wifi_mode;
   char wifi_ap_ssid[L_WIFI_SSID];
   char wifi_ap_pass[L_WIFI_PASS];
   time_t wifi_timeout;
   int  wifi_ap_wait;
   char admin_user[L_ADMIN_USER];
   char admin_pass[L_ADMIN_PASS];
   sio_port_t ports[MAX_PORTS];
};

extern bool config_load(void);
extern const char parity_to_str(sio_parity_t p);
extern Config_t cfg;

#endif
