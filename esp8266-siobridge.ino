/* This is just to keep arduino IDE happy. Everything lives in main.c */
#include "siobridge.h"
extern void main_loop(void);
extern void main_setup(void);

void loop(void) {
   main_loop();
}

void setup(void) {
   main_setup();
}
