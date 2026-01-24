// kernel/ui/clock.c
#include "clock.h"

static ScreenClock screen_clock = {0};

void clock_init(uint32_t target_fps)
{
    screen_clock.frame = 0;
    screen_clock.ms = 0;
    screen_clock.target_fps = target_fps > 0 ? target_fps : 30;
    screen_clock.frame_time_ms = 1000 / screen_clock.target_fps;
    screen_clock.last_update_ms = 0;
}

void clock_tick(void)
{
    // Called from PIT handler, typically every 1ms
    screen_clock.ms++;
}

ScreenClock *clock_get(void)
{
    return &screen_clock;
}

int clock_should_update(void)
{
    uint32_t elapsed = screen_clock.ms - screen_clock.last_update_ms;
    if (elapsed >= screen_clock.frame_time_ms)
    {
        screen_clock.last_update_ms = screen_clock.ms;
        screen_clock.frame++;
        return 1;
    }
    return 0;
}

void clock_reset_frame(void)
{
    screen_clock.frame = 0;
}

uint32_t clock_delta_ms(void)
{
    return screen_clock.ms - screen_clock.last_update_ms;
}
