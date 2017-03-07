#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

union my_float {
	struct {
#ifndef REVERSE
		uint16_t sign		:  1;
		uint16_t exponent	:  8;
		uint32_t mantissa	: 23;
#else
		uint32_t mantissa	: 23;
		uint16_t exponent	:  8;
		uint16_t sign		:  1;
#endif
	};
	float value;
};

int main(void) {
	union my_float f;
	f.value = -0.5;

	printf("sign:\t\t%d\n", f.sign);
	printf("exponent:\t%d\n", f.exponent);
	printf("mantissa:\t%u\n", f.mantissa);
	

	printf("\nsizeof(f):\t%u\n", sizeof(f));

	return 0;
}

/* vim:set ts=4 tw=79 syntax=c et: */
