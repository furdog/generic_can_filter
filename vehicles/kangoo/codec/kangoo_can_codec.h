#ifndef   KANGOO_CAN_CODEC_H
#define   KANGOO_CAN_CODEC_H

#include "bite.h"
#include <stdint.h>

/******************************************************************************
 * CODEC
 *
 * ⚠️ WARNING: REVERSE ENGINEERING IN PROGRESS ⚠️
 *
 * This software is the result of reverse engineering and is NOT an official
 * or supported product. The information and logic within this code are derived
 * from publicly available references, including 'Kangoo.dbc' and
 * 'CANBUS Fluence ZE - Battery.pdf', and supplemented by personal
 * investigation.
 *
 * 🚨 NOTE ON REFERENCES: 🚨
 * The original reference files, including the .dbc file and the Google Sheet,
 * have been found to contain inaccuracies and incomplete information. This
 * software attempts to correct and expand upon these references based on
 * further analysis and testing.
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
 * CANBUS Fluence ZE: https://docs.google.com/spreadsheets/d/1p_zEgy8RqvTV79mT4HFdyKinz3l3q-WlECp_W2ymePM/edit?gid=2136864555#gid=2136864555
 * Kangoo.dbc:        https://github.com/jamiejones85/DBC-files
 * 
 * This header is responsible for Kangoo CAN coder-encoder logic (codec).
 * It uses `bite.h` which employs a CAN-DBC-like notation to pack integers
 * into CAN data buffers.
 *
 *****************************************************************************/

/******************************************************************************
 * x424 (BATTERY)
 *****************************************************************************/
/* 7 BIT (Probably unsigned) BIG ENDIAN */
int16_t kangoo_x424_get_max_batt_temp_C(uint8_t *data)
{
	struct bite b = bite_init(data, BITE_ORDER_BIG_ENDIAN, 62, 7);

	return (int16_t)bite_get_u8(&b) - 40;
}

/******************************************************************************
 * x425 (BATTERY)
 *****************************************************************************/
/* 9 BIT (Probably unsigned) BIG ENDIAN */
float kangoo_x425_get_highest_cell_V(uint8_t *data)
{
	struct bite b = bite_init(data, BITE_ORDER_BIG_ENDIAN, 33, 9);

	return (bite_get_u16(&b) * 0.01f) + 1.0f;
}

/* 9 BIT (Probably unsigned) BIG ENDIAN */
float kangoo_x425_get_lowest_cell_V(uint8_t *data)
{
	struct bite b = bite_init(data, BITE_ORDER_BIG_ENDIAN, 48, 9);

	return (bite_get_u16(&b) * 0.01f) + 1.0f;
}

/******************************************************************************
 * x445 (BATTERY)
 *****************************************************************************/
/* 7 BIT (Probably unsigned) BIG ENDIAN */
int16_t kangoo_x445_get_max_batt_temp_C(uint8_t *data)
{
	struct bite b = bite_init(data, BITE_ORDER_BIG_ENDIAN, 31, 7);

	return (int16_t)bite_get_u8(&b) - 40;
}

void kangoo_x445_set_max_batt_temp_C(uint8_t *data, int16_t val)
{
	struct bite b = bite_init(data, BITE_ORDER_BIG_ENDIAN, 31, 7);

	bite_put_u8(&b, val + 40);
}

/* 9 BIT (Probably unsigned) BIG ENDIAN */
float kangoo_x445_get_highest_cell_V(uint8_t *data)
{
	struct bite b = bite_init(data, BITE_ORDER_BIG_ENDIAN, 24, 9);

	return (bite_get_u16(&b) * 0.01f) + 1.0f;
}

void kangoo_x445_set_highest_cell_V(uint8_t *data, float val)
{
	struct bite b = bite_init(data, BITE_ORDER_BIG_ENDIAN, 24, 9);

	bite_put_u16(&b, (val - 1.0f) * 100.0f * (uint16_t)1U);
}

/* 9 BIT (Probably unsigned) BIG ENDIAN */
float kangoo_x445_get_lowest_cell_V(uint8_t *data)
{
	struct bite b = bite_init(data, BITE_ORDER_BIG_ENDIAN, 47, 9);

	return (bite_get_u16(&b) * 0.01f) + 1.0f;
}

void kangoo_x445_set_lowest_cell_V(uint8_t *data, float val)
{
	struct bite b = bite_init(data, BITE_ORDER_BIG_ENDIAN, 47, 9);

	bite_put_u16(&b, (val - 1.0f) * 100.0f * (uint16_t)1U);
}

#endif /* KANGOO_CAN_CODEC_H */
