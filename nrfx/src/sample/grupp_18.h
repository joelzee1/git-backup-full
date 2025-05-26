#ifndef GRUPP_18
#define GRUPP_18
#include <nrfx_uarte.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrf_gpio.h>
#include <nrfx_systick.h>
#include <nrfx_rtc.h>


extern uint8_t uarte_buffer;
extern nrfx_uarte_t instance;
extern nrfx_rtc_t rtc_instance;

//void init_stuff(void);

void string_read(char string[]);

void uarte_write(char* data, int length);

void send_int(int converted_string,char string[]);

int read_int(char input_string[]);

#endif