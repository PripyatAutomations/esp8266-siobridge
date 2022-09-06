#include "siobridge.h"
extern void main_loop(void);
extern void main_setup(void);

void loop(void) {
   main_loop();
}

void setup(void) {
   main_setup();
}
