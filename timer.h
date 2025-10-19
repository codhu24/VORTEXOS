#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void init_timer(uint32_t frequency);
uint32_t get_system_ticks(void);
void sleep(uint32_t milliseconds);

#endif