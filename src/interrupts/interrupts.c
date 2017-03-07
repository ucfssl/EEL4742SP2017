#include <msp430.h>

#ifdef __GNUC__
/* GCC exposes the identifier __GNUC__. We check if this symbol is exposed and
 * if so, we declare interrupts the GCC way
 */
__attribute__((interrupt(TIMER0_A0_VECTOR)))
#else
/* Otherwise, we are using TI's compiler. Declare interrupts the TI way */
#pragma vector=TIMER0_A0_VECTOR
__interrupt
#endif
void timer0_isr(void) {
	P1OUT	^=	0xff;		/* toggle the logic state of the pin */
}

int main(void) {
	/* WDT+ Module Setup */
	WDTCTL 	=	WDTPW | 	/* writing to the WDT+ registers
							 * requires the proper password */
				WDTHOLD;	/* stop the WDT+ module from counting */

	/* GPIO setup */
	P1DIR	=	0xff;		/* All pins as output on P1 */
	P1OUT	=	0x01;		/* Initialize all pins as low except for
							 * the lowest one */
	
	/* Timer_A Setup -- details on Chapter 12 of slau144j */
	/* capture/compare control register */
	TACCTL0	=	CCIE;		/* enable capture/compare interrupt */
	/* capture/compare value register */
	TACCR0	=	62499;		/* set count top */
	/* timer control */
	TACTL	=	TASSEL1 |	/* clock source selection - use ACLK */
				MC0 |		/* up mode - count to TACCR0, then reset */
				ID_3;		/* clock divider - divide ACLK by 8 */
	
	/* shut off CPU and enable interrupts */
	__bis_SR_register(CPUOFF | GIE);
}

/* vim:set ts=4 tw=79 syntax=c et: */
