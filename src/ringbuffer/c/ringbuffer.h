#ifndef RINGBUFFER_H__
#define RINGBUFFER_H__

/* forward declare the type */
struct ringbuffer;

void rb_init(struct ringbuffer* rb);
int rb_put(struct ringbuffer* rb, int value);
const int rb_get(struct ringbuffer* rb);
int rb_empty(const struct ringbuffer* rb);
int rb_full(const struct ringbuffer* rb);


#define E_RBEMPTY	-1
#define E_RBFULL	-1

/* Bleh! The compiler needs to know the complete type if we want to allocate
 * the object ourselves. We could hide the implementation details if we provide
 * a rb_create() function that dynamically allocates the ring buffer, but we
 * should not be calling malloc() or the likes on an embedded system. It is
 * very easy to run out of memory in this land.
 */
#define BUFFERSIZE	10
struct ringbuffer {
	size_t head;
	size_t tail;
	size_t count;
	int container[BUFFERSIZE];
};

#endif	/* RINGBUFFER_H__ */
