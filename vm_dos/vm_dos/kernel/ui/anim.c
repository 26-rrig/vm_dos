// kernel/ui/anim.c
#include "anim.h"
#include "console.h"
#include "theme.h"

#define MAX_ANIMS 8
static anim_frame_fn ANIMS[MAX_ANIMS];
static int COUNT = 0;

void anim_register(anim_frame_fn fn)
{
    if (COUNT < MAX_ANIMS)
        ANIMS[COUNT++] = fn;
}

void anim_tick(uint32_t tick)
{
    for (int i = 0; i < COUNT; i++)
        ANIMS[i](tick);
}

// Spinner
static const char *SPIN = "|/-\\";
static uint32_t spin_idx = 0;
static void frame_spinner(uint32_t tick)
{
    if (tick % 5)
        return; // ~20 Hz
    spin_idx = (spin_idx + 1) & 3;
    char s[2] = {SPIN[spin_idx], 0};
    con_right_at(24, s);
}

// Clock (updates every second at 100Hz PIT)
extern uint32_t pit_ticks; // defined in PIT driver
static void frame_clock(uint32_t tick)
{
    if (tick % 100)
        return;

    uint32_t secs = tick / 100;
    int h = (secs / 3600) % 24;
    int m = (secs / 60) % 60;
    int s = secs % 60;

    // Build HH:MM:SS string without snprintf
    char tmp[9]; // HH:MM:SS
    tmp[0] = '0' + (h / 10);
    tmp[1] = '0' + (h % 10);
    tmp[2] = ':';
    tmp[3] = '0' + (m / 10);
    tmp[4] = '0' + (m % 10);
    tmp[5] = ':';
    tmp[6] = '0' + (s / 10);
    tmp[7] = '0' + (s % 10);
    tmp[8] = 0;

    con_right_at(24, tmp);
}

void anim_init(void)
{
    anim_register(frame_spinner);
    anim_register(frame_clock);
}
