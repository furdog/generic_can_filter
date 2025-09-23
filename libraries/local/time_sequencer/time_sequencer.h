#ifndef   TIME_SEQUENCER_H
#define   TIME_SEQUENCER_H

/******************************************************************************
 * CLASS
 *
 * Simple time sequencer that iterates throught array of timers and returns
 * simple uint16_t event codes (defined by user)
 *****************************************************************************/
struct ts_timer_u16
{
	uint16_t timer_ms; /* How long should we wait until event is emited? */
	uint16_t event;    /* User specified event */
};

struct ts_u16 {
	struct ts_timer_u16 *_timers;
	size_t               _count;

	size_t   _i; /* Current iteration */
	uint32_t _timer_ms; /* Time elapsed for current timer */
};

void ts_u16_init(struct ts_timer_u16 *self, struct ts_timer_u16 *tim_arr,
		 size_t count)
{
	self->_timers = NULL;
	self->_count  = 0u;

	self->_i        = 0u;
	self->_timer_ms = 0u;
}

#endif /* TIME_SEQUENCER_H*/
