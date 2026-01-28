// kernel/ui/toast.h
#pragma once
#include <stdint.h>
void toast_show(const char *msg, uint32_t duration_ticks); // e.g., 300 = 3s at 100Hz
void toast_tick(uint32_t tick);
