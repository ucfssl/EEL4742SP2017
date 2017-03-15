#include <avr/io.h>

/* AVR interrupt vector table:
 *  The linker script defines the section .vectors and places it at the start of
 *  program memory. In AVR, entries to the interrupt vector table are
 *  instructions which often serve as trampolines to the actual interrupt
 *  handler. This is unlike MSP430, where the CPU expects to find the address
 *  of the interrupt handler function. In AVR, execution starts from address
 *  0x0000 in program memory, which contains the reset vector. We populate that
 *  address with a jump instruction to the entry point of our program. The rest
 *  of the entries in the vector table is populated with our program. This
 *  would be an issue if interrupts were enabled, but we are not using those
 */
	.section .vectors
	.org 0					/* location of reset vector */
	.extern _start
jmp _start					/* reset vector */

