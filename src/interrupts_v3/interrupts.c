#include <msp430.h>

#include <interrupt.h>

/* Instead of having preprocessor directives here to select which interrupt
 * vector declaration we need to use, we use the ISR() macro that was declared
 * in the interrupt.h header. This makes our code easier to read at the expense
 * of having an extra header file floating around.
 */
ISR(TIMER0_A0_VECTOR) {
	static int total_triggers;
	P1OUT	^=	0xff;		/* toggle the logic state of the pin */

	if(total_triggers == 10) {
		/* if this is the 10th time service the interrupt, leave the CPU on
		 * when exiting the interrupt
		 */
		__bic_SR_register_on_exit(CPUOFF);
	} else if(total_triggers < 10) {
		/* otherwise, count how many times we have done this */
		total_triggers++;
	}
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
	
	/* we can actually get here now! */
	P1OUT	=	0xff;		/* all pins set to logic 1 */
	while(1) {
		;					/* the CPU is on, we need to stay on an infinite
							 * loop */
	}
}

/* vim:set ts=4 tw=79 syntax=c et: */
