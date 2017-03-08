#include <stddef.h>

#include "ringbuffer.h"


void rb_init(struct ringbuffer* rb) {
	rb->head = 0;
	rb->tail = BUFFERSIZE - 1;
	rb->count = 0;
}

int rb_put(struct ringbuffer* rb, int value) {
	if(rb_full(rb)) {
		return E_RBFULL;
	}

	rb->count++;
	rb->container[rb->head++] = value;
	rb->head %= BUFFERSIZE;

	return 0;
}

const int rb_get(struct ringbuffer* rb) {
	if(rb_empty(rb)) {
		return E_RBEMPTY;
	}

	rb->count--;
	rb->tail++;
	rb->tail %= BUFFERSIZE;

	return rb->container[rb->tail];
}

int rb_empty(const struct ringbuffer* rb) {
	return rb->count == 0;
}

int rb_full(const struct ringbuffer* rb) {
	return rb->count == BUFFERSIZE;
}

