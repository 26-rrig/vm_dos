// kernel/arch/x86/pit.h
#pragma once
#include <stdint.h>

void pit_init(int hz);
void pit_sleep(uint32_t ticks);

// Global tick counter incremented by the PIT IRQ handler
extern volatile uint32_t pit_ticks;

#pragma once
void pic_remap(void);
