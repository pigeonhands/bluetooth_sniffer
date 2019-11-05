#pragma once

#include "nrfx.h"

typedef void (*timer_on_tick_f)();

void timer_init(timer_on_tick_f cb);
void timer_start(uint16_t timeout_ms);
void timer_stop();