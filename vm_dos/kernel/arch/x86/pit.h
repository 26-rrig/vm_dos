#pragma once
#include <stdint.h>
void pit_sleep(uint32_t ticks);
void pit_init(int hz);
extern volatile uint32_t pit_ticks;
