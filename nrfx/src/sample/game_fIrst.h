#ifndef game_first
#define game_first
#include "grupp_18.h"

extern int answer;
extern int state;
extern int now;
extern char temp[100];

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context);

void charthers(int choice);

int function_addition(int diff,int time);

int function_subtraction(int diff,int time);

int function_multiplication(int diff,int time);

int countdown(int seconds,int level, int facit);

#endif