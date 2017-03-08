#include <iostream>

#include "ringbuffer.hpp"

int main() {
	ringbuffer<int, 10> rb;
	int i = 0;
	
	// fill the ring buffer
	while(!rb.full()) {
		std::cout << "adding: " << i << std::endl;
		rb.put(i++);
	}

	// throws a bad_access exception
	// rb.put(i);

	// grab elements from the ring buffer
	while(!rb.empty()) {
		std::cout << rb.get() << std::endl;
	}

	// throws a bad_access exception
	// std::cout << rb.get() << std::endl;
	return 0;
}
