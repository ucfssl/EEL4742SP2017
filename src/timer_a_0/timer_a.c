#include <stdint.h>

#include <msp430.h>

/* We create two preprocessor macros defining the position of the LEDs. We hope
 * that this makes our code more readable.
 */
#define LED1    BIT0
#define LED2    BIT6

#ifdef __GNUC__
/* GCC exposes the identifier __GNUC__. We check if this symbol is exposed and
 * if so, we declare interrupts the GCC way.
 */
__attribute__((interrupt(TIMER0_A0_VECTOR)))
#else
/* Otherwise, we are using TI's compiler. Declare interrupts the TI way */
#pragma vector=TIMER0_A0_VECTOR
__interrupt
#endif
void timer0_isr(void) {
    /* The ``static'' storage class specifier gives the variable permanent
     * store, so it is not allocated in the stack every time the interrupt
     * handler executes. This actually saves execution time from our
     * interrupt, as we do not need to do any initialization on every interrupt
     * execution. The ``const'' type qualifier further tells the compiler that
     * the data is read only, so it gets placed in .rodata and not copied into
     * RAM. Since the variable is local, the symbol is not visible from outside
     * the interrupt. This trick is nice to use if we want some data that is
     * local to a function, but we do not want to initialize it every time we
     * execute the function.
     */
    static const uint8_t pattern[] = {
            LED1 + LED2,        /* LED1 and LED2 on at the same time */
            LED1,               /* LED1 only */
            0,                  /* no LED is on */
            LED2                /* LED2 only */
        };

    /* We also declare another variable with permanent store, but we do not
     * initialize it. Remember that variables that have permanent store but are
     * not explicitly initialized go into the .bss section and are initialized
     * by the runtime to 0.
     */
    static uint8_t out;
	P1OUT	=	pattern[out++];	/* set the new patter on the output pin and
                                 * increment the counter */
    out &= 0x3;                 /* Bitmasking with 0x3 is equivalent of
                                 * performing a modulo 4 division. There are
                                 * only four patterns in our array [0, 1, 2,
                                 * 3], as such we must bind out to this range.
                                 * Modulo 4 division would fix this problem but
                                 * the MSP430 does not have a hardware divide
                                 * instruction. The compiler would translate
                                 * the division to the equivalent assembly
                                 * code, but we avoid having this happen by
                                 * performing a bitmask. */
}

int main(void) {
	/* WDT+ Module Setup */
	WDTCTL 	=	WDTPW | 	/* writing to the WDT+ registers
							 * requires the proper password */
				WDTHOLD;	/* stop the WDT+ module from counting */

	/* GPIO setup */
	P1DIR	=	0xff;		/* All pins as output on P1 */
	P1OUT	=	0x00;		/* Initialize all pins as low */
	
	/* Timer_A Setup -- details on Chapter 12 of slau144j */
	/* capture/compare control register */
	TACCTL0	=	CCIE;		/* enable capture/compare interrupt */
	/* capture/compare value register */
	TACCR0	=	62499;		/* set count top */

	TACTL	=	TASSEL1 |	/* clock source selection - use SMCLK */
				MC0 |		/* up mode - count to TACCR0, then reset */
				ID_3;		/* clock divider - divide ACLK by 8 */
	
	/* shut off CPU and enable interrupts */
	__bis_SR_register(CPUOFF | GIE);
}

/* vim:set ts=4 tw=79 syntax=c et: */
