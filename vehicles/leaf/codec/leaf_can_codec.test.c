#include "leaf_can_codec.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void leaf_ze0_x5BC_test_invalid()
{
	uint8_t sample_ze0_x5BC_0[] = { /* Set all data to 0xFF */
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	/* Must fail validation */
	assert(!leaf_ze0_x5BC_is_valid(sample_ze0_x5BC_0));
}

void leaf_ze0_x5BC_test_capacity()
{
	uint16_t remaining_gids;
	uint16_t full_gids;

	uint8_t sample_ze0_x5BC_0[] = {
		0x5D, 0xC0, 0xDC, 0x78, 0xC8, 0x13, 0x1F, 0xFF };
	uint8_t sample_ze0_x5BC_1[] = {
		0x55, 0x80, 0xDC, 0x78, 0xC8, 0x00, 0x1F, 0xFF };

	assert(leaf_ze0_x5BC_is_valid(sample_ze0_x5BC_0));

	assert(leaf_ze0_x5BC_get_capacity_mux(sample_ze0_x5BC_0) ==
	       LEAF_ZE0_X5BC_CAPACITY_MUX_FULL);

	assert(leaf_ze0_x5BC_get_capacity_mux(sample_ze0_x5BC_1) ==
	       LEAF_ZE0_X5BC_CAPACITY_MUX_REMAINING);

	full_gids = leaf_ze0_x5BC_get_capacity_gids(sample_ze0_x5BC_0);
	remaining_gids = leaf_ze0_x5BC_get_capacity_gids(sample_ze0_x5BC_1);

	printf("full_gids: %u (~%uWh)\n", full_gids, full_gids * 80);
	assert(full_gids == 375);

	printf("remn_gids: %u (~%uWh)\n", remaining_gids, remaining_gids * 80);
	assert(remaining_gids == 342);
}

void leaf_ze0_x1DB_test_measurements()
{
	uint16_t voltage_V_x2;
	int16_t  current_A_x2;

	uint8_t sample_ze0_x1DB_invalid[] = { /* Set all data to 0xFF */
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	uint8_t sample_ze0_x1DB[] = {
		0xFF, 0xC0, 0xC2, 0x2A, 0x5B, 0x00, 0x02, 0x60 };

	assert(!leaf_ze0_x1DB_is_valid(sample_ze0_x1DB_invalid));
	assert(leaf_ze0_x1DB_is_valid(sample_ze0_x1DB));

	voltage_V_x2 = leaf_ze0_x1DB_get_voltage_V_x2(sample_ze0_x1DB);
	current_A_x2 = leaf_ze0_x1DB_get_current_A_x2(sample_ze0_x1DB);

	printf("voltage_V_x2: %u (%fV)\n", voltage_V_x2, voltage_V_x2 / 2.0f);
	printf("current_A_x2: %i (%fA)\n", current_A_x2, current_A_x2 / 2.0f);
}

int main()
{
	leaf_ze0_x5BC_test_invalid();
	leaf_ze0_x5BC_test_capacity();
	leaf_ze0_x1DB_test_measurements();

	return 0;
}
