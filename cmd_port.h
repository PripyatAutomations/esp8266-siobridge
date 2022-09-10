static bool cmd_port_add(Stream *ch, const char *args[]) {
}

static bool cmd_port_delete(Stream *ch, const char *args[]) {
}

static bool cmd_port_list(Stream *ch, const char *args[]) {
   sio_port_t *p;

   for (int i = 0; i < MAX_PORTS; i++) {
      p = &cfg.ports[i];

      if (p->configured) {
         ch->printf("sio%d: %d%c%d@%d (Pins: tx=%d,rx=%d,dtr=%d,rts=%d,cts=%d) %sabled telnet port: %d refcnt: %d (%d unread)\r\n", i,
                    p->data_bits, parity_to_str(p->parity), p->stop_bits, p->baud_rate,
                    p->tx_pin, p->rx_pin, p->dtr_pin, p->rts_pin, p->cts_pin,
                    p->telnet_port, p->refcnt, p->unread_buffers,
                    (p->enabled == false ? "en" : "dis"));
      }
   }
}

static bool cmd_connect(Stream *ch, const char *args[]) {
   return false;
}
