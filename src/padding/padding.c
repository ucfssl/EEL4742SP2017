#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct s1 {
	int16_t c;
	int64_t a;
	int32_t b;
};

struct s2 {
	int64_t a;
	int32_t b;
	int16_t c;
};

int main(void) {
	printf("sizeof(struct s1) = %d\n", sizeof(struct s1));
	printf("sizeof(struct s2) = %d\n", sizeof(struct s2));

	return EXIT_SUCCESS;
}

/* vim:set ts=4 tw=79 syntax=c et: */
