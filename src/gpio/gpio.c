#include <msp430.h>

/* MSP-EXP430G2 board specific */
#define LED_PIN		BIT0	/* LED1 is connected to P1.0 */
#define BUTTON_PIN	BIT3	/* S2 is connected to P1.3 */

void main(void) {
	WDTCTL = WDTPW | WDTHOLD;	/* stop the watchdog timer */

	P1DIR = (LED_PIN);			/* P1.0 is an output */
	P1REN = (BUTTON_PIN);		/* enable resistor */
	P1OUT = (BUTTON_PIN);		/* as a pullup */

	while(1) {
		if(P1IN & (BUTTON_PIN)) {
			/* if the button not pressed, turn on the LED */
			P1OUT |= (LED_PIN);
		} else {
			/* otherwise, turn off the LED */
			P1OUT &= ~(LED_PIN);
		}
	}
}
