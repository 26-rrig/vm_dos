// kernel/ui/clock.h
#pragma once
#include <stdint.h>

typedef struct
{
    uint32_t frame;          // Current frame counter
    uint32_t ms;             // Milliseconds elapsed
    uint32_t target_fps;     // Target frames per second
    uint32_t frame_time_ms;  // Time per frame in milliseconds
    uint32_t last_update_ms; // Last screen update time
} ScreenClock;

/**
 * Initialize the screen clock with target FPS
 */
void clock_init(uint32_t target_fps);

/**
 * Update the clock (call from PIT handler)
 */
void clock_tick(void);

/**
 * Get the current screen clock
 */
ScreenClock *clock_get(void);

/**
 * Check if enough time has passed for next frame
 */
int clock_should_update(void);

/**
 * Reset frame counter
 */
void clock_reset_frame(void);

/**
 * Get elapsed time since last update in milliseconds
 */
uint32_t clock_delta_ms(void);
