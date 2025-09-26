#include "leaf_can_codec.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void leaf_ze0_x5BC_test_invalid()
{
	uint8_t sample_ze0_x5BC_0[] = { 0xFF }; /* Set all data to 0xFF */

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

int main()
{
	leaf_ze0_x5BC_test_invalid();
	leaf_ze0_x5BC_test_capacity();

	return 0;
}
