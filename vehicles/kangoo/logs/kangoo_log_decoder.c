#include "kangoo_can_codec.h"
#include "canary_log_reader.h"

#include <assert.h>
#include <stdio.h>

/******************************************************************************
 * MONITORED VARIABLES
 *****************************************************************************/
/* Monitor variables (maximum supported unsigned integer type) */
struct mon_var_u32 {
	uint32_t _v;

	bool dirty;
};

void mon_var_u32_init(struct mon_var_u32 *self)
{
	self->_v    = 0u;
	self->dirty = false;
}

void mon_var_u32_set(struct mon_var_u32 *self, uint32_t v)
{
	if (self->_v != v) {
		self->_v    = v;
		self->dirty = true;
	}
}

uint32_t mon_var_u32_get(struct mon_var_u32 *self)
{
	return self->_v;
}

/* Monitor variables (floating point type) */
struct mon_var_float {
	float _v;

	bool dirty;
};

void mon_var_float_init(struct mon_var_float *self)
{
	self->_v    = 0.0f;
	self->dirty = false;
}

void mon_var_float_set(struct mon_var_float *self, float v)
{
	if (self->_v != v) {
		self->_v    = v;
		self->dirty = true;
	}
}

float mon_var_float_get(struct mon_var_float *self)
{
	return self->_v;
}

/******************************************************************************
 * BMS STATE
 *****************************************************************************/
struct kangoo_bms_state {
	struct mon_var_u32 max_batt_temp_C_424;

	struct mon_var_float hicell_V_425;
	struct mon_var_float locell_V_425;

	struct mon_var_u32   max_batt_temp_C_445;
	struct mon_var_float hicell_V_445;
	struct mon_var_float locell_V_445;
} bms_state;

void bms_state_init(struct kangoo_bms_state *self)
{
	mon_var_u32_init(&self->max_batt_temp_C_424);

	mon_var_float_init(&self->hicell_V_425);
	mon_var_float_init(&self->locell_V_425);

	mon_var_u32_init(  &self->max_batt_temp_C_445);
	mon_var_float_init(&self->hicell_V_445);
	mon_var_float_init(&self->locell_V_445);
}

void kangoo_frame_decode(struct kangoo_bms_state *self,
			 struct canary_log_reader_frame *frame)
{
	switch (frame->id) {
	case 0x424u:
		mon_var_u32_set(&self->max_batt_temp_C_424,
				kangoo_x424_get_max_batt_temp_C(frame->data));
		break;

	case 0x425u:
		mon_var_float_set(&self->hicell_V_425,
				kangoo_x425_get_highest_cell_V(frame->data));
		mon_var_float_set(&self->locell_V_425,
				kangoo_x425_get_lowest_cell_V(frame->data));
		break;

	case 0x445u:
		mon_var_u32_set(&self->max_batt_temp_C_445,
				kangoo_x445_get_max_batt_temp_C(frame->data));
		mon_var_float_set(&self->hicell_V_445,
				kangoo_x445_get_highest_cell_V(frame->data));
		mon_var_float_set(&self->locell_V_445,
				kangoo_x445_get_lowest_cell_V(frame->data));
		break;
	}
}

void kangoo_print_bms_state_fancy(struct kangoo_bms_state *self)
{
	const char *grn = "\e[0;32m";
	const char *rst = "\e[0m";

	const char *col = NULL;

	if (self->max_batt_temp_C_424.dirty) { col = grn; } else { col = rst; }
	printf("%s%3i   ", col, (int16_t)
		mon_var_u32_get(&bms_state.max_batt_temp_C_424));

	if (self->hicell_V_425.dirty) { col = grn; } else { col = rst; }
	printf("%s%0.3f   ", col, mon_var_float_get(&bms_state.hicell_V_425));
	if (self->locell_V_425.dirty) { col = grn; } else { col = rst; }
	printf("%s%0.3f   ", col, mon_var_float_get(&bms_state.locell_V_425));

	if (self->max_batt_temp_C_445.dirty) { col = grn; } else { col = rst; }
	printf("%s%3i   ", col, (int16_t)
		mon_var_u32_get(&bms_state.max_batt_temp_C_445));
	if (self->hicell_V_445.dirty) { col = grn; } else { col = rst; }
	printf("%s%0.3f   ", col, mon_var_float_get(&bms_state.hicell_V_445));
	if (self->locell_V_445.dirty) { col = grn; } else { col = rst; }
	printf("%s%0.3f   ", col, mon_var_float_get(&bms_state.locell_V_445));

	self->max_batt_temp_C_424.dirty = false;
	self->hicell_V_425.dirty = false;
	self->locell_V_425.dirty = false;

	self->max_batt_temp_C_445.dirty = false;
	self->hicell_V_445.dirty = false;
	self->locell_V_445.dirty = false;

	printf("\n");
}

int main()
{
	int c;
	struct canary_log_reader c_inst;

	FILE *file = fopen("canary/kangoo_idle.csv", "r");

	assert(file);
	
	canary_log_reader_init(&c_inst);

	bms_state_init(&bms_state);

	c = getc(file);
	while (!feof(file)) {
		enum canary_log_reader_event ev =
					    canary_log_reader_putc(&c_inst, c);
		if (ev ==
		    CANARY_LOG_READER_EVENT_FRAME_READY) {
			/*canary_print_frame(&c_inst);*/

			/* TODO Proper frame read*/
			kangoo_frame_decode(&bms_state, &c_inst._frame);

			kangoo_print_bms_state_fancy(&bms_state);
		}

		if (ev == CANARY_LOG_READER_EVENT_ERROR) {
			printf("err, state: %i, flags: %i\n",
				c_inst._estate, c_inst._eflags); fflush(0);
		}

		c = getc(file);
	}

	printf("FINISHED, TOTAL_FRAMES: %llu\n", c_inst._total_frames);

	return 0;
}
