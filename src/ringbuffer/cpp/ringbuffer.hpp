#ifndef RINGBUFFER_HPP__
#define RINGBUFFER_HPP__

#include <cstddef>

template <class T, size_t size>
class ringbuffer {
	public:
		void put(const T& value) {
			if(this->full()) {
				throw bad_access();
			}

			count++;
			container[head++] = value;
			head %= size;
		}

		const T& get() {
			if(this->empty()) {
				throw bad_access();
			}
			count--;
			tail++;
			tail %= size;
			return container[tail];
		}

		bool empty() {
			return count == 0;
		}

		bool full() {
			return count == size;
		}
	private:
		size_t head = 0, tail = size - 1, count = 0;
		T container[size];

		class bad_access : public std::exception {
			public:
				const char* what() const noexcept {
					return "attempted erroneous access";
				}
		};
};

#endif	// RINGBUFFER_HPP__
