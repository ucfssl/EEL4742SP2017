#include <stddef.h>

#include <avr/io.h>

/* this global array is explicitly initialized, it goes into .data */
unsigned char source[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };

/* this global array is not explicitly initialized, it goes into .bss */
unsigned char target[sizeof(source)/sizeof(source[0])];

int main(void) {
	/* copy source into target */
	for(size_t i = 0; i < sizeof(source)/sizeof(source[0]); i++) {
		target[i] = source[i];
	}
	while(1);	/* do not exit main */
}
