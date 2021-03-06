#include  "timer.h"

#define TIMER_Reg NRF_TIMER1
#define TIMER_Irq TIMER1_IRQHandler

#define TIMER_COMPARE_READY(REG, CMPINDEX) ((REG->EVENTS_COMPARE[CMPINDEX] != 0) && ((REG->INTENSET & TIMER_INTENSET_COMPARE ## CMPINDEX ## _Msk) != 0))
#define TIMER_COMPARE_CLEAR(REG, CMPINDEX) REG->EVENTS_COMPARE[CMPINDEX] = 0

#define TIMER_PRESCALER 9

static struct {
	timer_on_tick_f cb;
}ctx;

void timer_init(timer_on_tick_f cb) {
	TIMER_Reg->MODE			= TIMER_MODE_MODE_Timer;
	TIMER_Reg->BITMODE		= TIMER_BITMODE_BITMODE_32Bit;
	TIMER_Reg->PRESCALER	= TIMER_PRESCALER;
	TIMER_Reg->SHORTS		= TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos;
	
	TIMER_Reg->INTENSET		= (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos);
	NVIC_EnableIRQ(TIMER1_IRQn);
	
	ctx.cb = cb;
}

__STATIC_INLINE uint32_t timer_ms_to_ticks(uint32_t              time_ms,
                                               uint32_t frequency) {
	// The "frequency" parameter here is actually the prescaler value, and the
	// timer runs at the following frequency: f = 16000 kHz / 2^prescaler.
	uint32_t prescaler = (uint32_t)frequency;
	uint64_t ticks = ((time_ms * 16000ULL) >> prescaler);
	NRFX_ASSERT(ticks <= UINT32_MAX);
	return (uint32_t)ticks;
}

void timer_start(uint16_t timeout_ms) {
	TIMER_Reg->CC[0] = timer_ms_to_ticks(timeout_ms, TIMER_PRESCALER);
	TIMER_Reg->TASKS_CLEAR = 1; 
	TIMER_Reg->TASKS_START = 1;
}
void timer_stop() {
	TIMER_Reg->TASKS_STOP = 1;
}

#ifdef __cplusplus
extern "C"
#endif
void TIMER_Irq(void) {
	
	if (TIMER_COMPARE_READY(TIMER_Reg, 0)) {
		if (ctx.cb) {
			ctx.cb();
		}
		TIMER_COMPARE_CLEAR(TIMER_Reg, 0);
	}
	
}

