#include <stdint.h>

#include "morse.h"

const uint8_t morse[][17] = {
		#include "morsecode.h"
	};

const uint8_t space[5] = {4};
