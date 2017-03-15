#include <avr/io.h>

	.section .text._start
	.extern main
	.globl _start		/* we need _start to be global so that the linker can
						 * resolve the symbol from another file
						 */
_start:
	/* The C compiler expects r1 to be zeroed. We need to zero it */
	eor r1, r1
	/* we also zero the status register. */
	out _SFR_IO_ADDR(SREG), r1
	/* We must initialize the stack pointer before using the call instruction,
	 * otherwise, we really do not know where the return address stored in the
	 * stack by the call instruction would be stored at.
	 */
	ldi r17, hi8(RAMEND)	/* get the end of RAM */
	ldi r16, lo8(RAMEND)	/* RAMEND is defined in <avr/io.h> */
	out _SFR_IO_ADDR(SPH), r17			/* and initialize the stack pointer */
	out _SFR_IO_ADDR(SPL), r16

	/* We just let the code fall through to our next step. Doing this saves us
	 * from having to call a function and return from it.
	 */
__initialize_data:
	/* we store the end address of the .data section in program memory */
	ldi r17, hi8(__data_load_end)
	ldi r16, lo8(__data_load_end)
	/* and we initialize the Z pointer to the start of the .data section in
	 * program memory
	 */
	ldi r31, hi8(__data_load_start)
	ldi r30, lo8(__data_load_start)
	/* we initialzie the Z pointer to the start of .data in data memory */
	ldi r29, hi8(__data_start)
	ldi r28, lo8(__data_start)
	/* We jump ahead to the comparison and come back if we have to copy data.
	 * This allows us to use the cpse [compare skip if equal] instruction. Most
	 * compilers will issue code like the one below for loops. The construct
	 * allows the compiler to make the assumption that the body of the loop
	 * will execute at least once, which allows for an extra set of
	 * optimizations to be made.
	 */
	jmp 1f
	/* copy loop */
2:	lpm r0, Z+		/* Z gets autoincremented after lpm gets executed */
	st Y+, r0		/* Y gets autoincremented after st gets executed */
	/* Check whether or not the Z pointer has reached __data_load_end. If it
	 * has not, the cpse instruction will not skip the jumps and the loop
	 * continues to execute. Otherwise, the jumps get skipped and the program
	 * continues.
	 */
1:	cpse r31, r17
	jmp 2b
	cpse r30, r16
	jmp 2b

__initialize_bss:
	/* similarly, we keep track of where the .bss section ends */
	ldi r17, hi8(__bss_end)
	ldi r16, lo8(__bss_end)
	/* in theory, .bss should start after .data, but we will not trust this */
	ldi r29, hi8(__bss_start)
	ldi r28, lo8(__bss_start)
	jmp 1f
	/* zero .bss loop */
2:	st Y+, r1	/* r1 is zero from above */
	/* Same as above, we check if the Y pointer has reached __data_end. If it
	 * has, then the cpse instruction will skip the jumps and the program
	 * continues. Otherwise, the loop continues to execute.
	 */
1:	cpse r29, r17
	jmp 2b
	cpse r28, r16
	jmp 2b

	/* At this point, we have the stack pointer set up. We also have the .data and
	 * .bss sections properly initialized in data memory. It is now safe to
	 * call main() in our C code.
	 */
_call_main:
	/* call the main function of our program */
	call main

	/* in case we do exit main... */
__halt:
1:	cli					/* clear global interrupt flag, disabling interrupts */
	ldi r16, _BV(SE)	/* set the sleep enable bit */
	out _SFR_IO_ADDR(MCUCR), r16	/* in the MCU control register */
	sleep			/* and enter sleep mode */
	jmp 1b			/* one final safeguard */
