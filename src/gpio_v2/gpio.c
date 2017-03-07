#include <msp430.h>
#include <interrupt.h>

/* MSP-EXP430G2 board specific */
#define LED_PIN		BIT0	/* LED1 is connected to P1.0 */
#define BUTTON_PIN	BIT3	/* S2 is connected to P1.3 */

ISR(PORT1_VECTOR) {
	/* we check that the interrupt was indeed triggered by the button pin */
	if(P1IFG & (BUTTON_PIN)) {
		/* if it was, we change the transition of the interrupt */
		P1IES	^= (BUTTON_PIN);
		/* and toggle the LED */
		P1OUT	^= (LED_PIN);
	}

	/* we must manually clear the interrupt flag register before exiting */
	P1IFG = 0x00;
}

void main(void) {
	WDTCTL	= WDTPW | WDTHOLD;	/* stop the watchdog timer */

	P1DIR	= (LED_PIN);		/* P1.0 is an output */
	P1REN	= (BUTTON_PIN);		/* enable resistor on button */
	P1OUT	= (BUTTON_PIN);		/* as a pullup */

	P1IES 	= (BUTTON_PIN);		/* P1IFG bit for button pin set
								 * when transitioning from high to low
								 */
	P1IE	= (BUTTON_PIN);		/* enable the interrupt */

	/* shut off the CPU while enabling interrupts */
	__bis_SR_register(CPUOFF | GIE);
}
