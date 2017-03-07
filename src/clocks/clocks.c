#include <msp430.h>

#define XTOUT	BIT7
#define XTIN	BIT6

#define ACLK	BIT0
#define SMCLK	BIT4

int main(void) {
	/* Halt the watchdog timer */
	WDTCTL	=	WDTPW | WDTHOLD;

	/* Configure pins for external crystal. For details, see pages 52 to 55 of
	 * document number slas735.
	 */
	P2DIR	=	XTOUT;				/* XTOUT as output */
	P2SEL	=	XTOUT | XTIN;		/* first function select */
	P2SEL2	&=	~(XTOUT | XTIN);	/* second function select */

	/* Configure I/O to dump clocks. For details, see pages 42, 43, 46, and 47
	 * of document number slas735.
	 */
	P1DIR	=	ACLK | SMCLK;		/* set pins as output */
	P1SEL	=	ACLK | SMCLK;		/* first function select */
	P2SEL2	&=	~(ACLK | SMCLK);	/* second function select */

	/* turn off CPU */
	__bis_SR_register(CPUOFF);
}
