#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/******************************************************************************
 * CLASS
 *  Journal is simple FIFO, data agnostic helper tool that provides journaling
 *  capabilities. It's useful for event and data logging.
 *  The data buffer, as well as data format is fully user specified.
 *  To comply with MISRA specs use of memcpy is highly encouraged!
 *
 * WARNING
 *  Journal data is not intended to be used outside of logging purposes,
 *  There's no guarantee that any event will be journaled at 100% of time
 *  So worst cases must be assumed
 *****************************************************************************/
struct journal_entry {
	uint8_t type; /* User specified data type */
	uint8_t len;  /* Entry Data length (255 bytes max) */

	void   *data; /* User specified data */
};

struct journal {
	uint8_t *_data;
	size_t   _capacity;

	size_t   _head_position; /* Increments on pop  */
	size_t   _tail_position; /* Increments on push */

	/* Public, access only after successful pop operation */
	struct journal_entry entry;
};

void journal_init(struct journal *self, uint8_t *data, size_t capacity)
{
	self->_data     = data;
	self->_capacity = capacity;

	self->_head_position = 0u;
	self->_tail_position = 0u;
}

/* Will push entry into a journal. Wont do anything if size exeeds capacity */
void journal_push(struct journal *self, uint8_t type, void *data, uint8_t len)
{
	size_t header_size  = sizeof(self->entry.type) +
			      sizeof(self->entry.len);

	size_t entry_size   = header_size + len;
	size_t new_tail_pos = self->_tail_position + entry_size;

	if (new_tail_pos < self->_capacity) {
		self->_data[self->_tail_position] = type;
		self->_tail_position += sizeof(self->entry.type);

		self->_data[self->_tail_position] = len;
		self->_tail_position += sizeof(self->entry.len);

		(void)memcpy(&self->_data[self->_tail_position], data, len);
		self->_tail_position = new_tail_pos;
	}
}

/* Will return true if operation was successful */
bool journal_pop(struct journal *self)
{
	bool result = false;

	size_t header_size  = sizeof(self->entry.type) +
			      sizeof(self->entry.len);

	if ((self->_head_position + header_size) <= self->_tail_position) {
		self->entry.type = self->_data[self->_head_position];
		self->_head_position += sizeof(self->entry.type);

		self->entry.len = self->_data[self->_head_position];
		self->_head_position += sizeof(self->entry.len);

		self->entry.data = &self->_data[self->_head_position];
		self->_head_position += self->entry.len;

		result = true;
	}

	return result;
}

/* Empties journal */
void journal_flush(struct journal *self)
{
	self->_head_position = 0u;
	self->_tail_position = 0u;
}
