// kernel/ui/anim.h
#pragma once
#include <stdint.h>
typedef void (*anim_frame_fn)(uint32_t tick);
void anim_register(anim_frame_fn fn);
void anim_tick(uint32_t tick);
void anim_init(void);
