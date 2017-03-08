#include <stdio.h>
#include <stdlib.h>

#include "ringbuffer.h"

int main(void) {
	struct ringbuffer rb;
	int i;

	rb_init(&rb);

	while(!rb_full(&rb)) {
		printf("adding: %d\n", i);
		rb_put(&rb, i++);
	}

	while(!rb_empty(&rb)) {
		printf("%d\n", rb_get(&rb));
	}

	return EXIT_SUCCESS;
}
