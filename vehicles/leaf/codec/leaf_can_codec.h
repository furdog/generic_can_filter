#ifndef   LEAF_CAN_CODEC_H
#define   LEAF_CAN_CODEC_H

#include "bite.h"
#include <stdint.h>

/******************************************************************************
 * CODEC
 *
 * ⚠️ WARNING: REVERSE ENGINEERING IN PROGRESS ⚠️
 *
 * This software is the result of reverse engineering and is NOT an official
 * or supported product. The information and logic within this code are derived
 * from publicly available references.
 *
 * 🚨 NOTE ON REFERENCES: 🚨
 * 
 * The original sources have been found to contain inaccuracies
 * and incomplete information. This software attempts to correct and expand
 * based on further analysis and testing.
 * 
 * The author, furdog, is not affiliated with the creators of the original
 * systems or reference files. This code may contain inaccuracies or omissions.
 *
 * 🚨 USE AT YOUR OWN RISK. 🚨
 *
 * By using this software, you acknowledge that you do so at your own risk.
 * The author and contributors are not liable for any damage, or
 * other issues that may arise from its use. You are responsible for ensuring
 * compliance with all applicable laws and regulations regarding reverse
 * engineering and the use of this software.
 *
 * Original references:
 * https://github.com/dalathegreat/leaf_can_bus_messages
 * https://github.com/dalathegreat/Battery-Emulator
 * https://github.com/dalathegreat/Nissan-LEAF-Battery-Upgrade
 *
 * This header is responsible for Leaf CAN coder-encoder logic (codec).
 * It uses `bite.h` which employs a CAN-DBC-like notation to pack integers
 * into CAN data buffers.
 *
 * Additional note:
 * _ze0, _ze1, _aze0, etc, prefixes fill be used for various vehicle versions
 * no prefix means the message is applicable for all vehicles. There will
 * be also special cases (that should be properly documented).
 * Some messages will contain those prefixes even though they're not version
 * dependent. This means that the author (furdog) can not confirm behaviour on different
 * vehicle version.
 *****************************************************************************/

/******************************************************************************
 * BO_ 1468 x5BC: 8 HVBAT
 *****************************************************************************/
/* Tells if x5bc message is valid (booted up) */
bool leaf_x5bc_is_valid(uint8_t *data)
{
	bool result = false;

	/* If LBC not booted up - exit */
	if (data[0u] != 0xFFu) {
		result = true;
	}

	return result;
}

/* Capacity MUX (poorly documented) */
enum leaf_x5bc_capacity_mux {
	LEAF_X5BC_CAPACITY_MUX_REMAINING,
	LEAF_X5BC_CAPACITY_MUX_FULL
};

enum leaf_x5bc_capacity_mux leaf_x5bc_get_capacity_mux(uint8_t *data) {
	/* leaf_x5bc_get_capacity_gids will show
	 * either full or remaining capacity based on this mux: */
	return ((data[5u] & 0x10u) > 0u) ? LEAF_X5BC_CAPACITY_MUX_REMAINING :
					   LEAF_X5BC_CAPACITY_MUX_FULL;
}

/* SG_ LB_Remain_Capacity :
 * 	7|10@0+ (1,0) [0|500] "gids" Vector__XXX
 * WARNING: returned value depends on leaf_x5bc_get_capacity_mux */
uint16_t leaf_x5bc_get_capacity_gids(uint8_t *data) {
	struct bite b = bite_init(data, BITE_ORDER_DBC_0, 7u, 10u);

	return bite_get_u16(&b);
}

#endif /* LEAF_CAN_CODEC_H */
