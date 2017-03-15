#include <msp430.h>

#include "config.h"
#include "gpio.h"
#include "interrupt.h"

ISR(PORT1_VECTOR) {
	/* if we triggered this because the button */
	if(BPIFG & BUTTON) {
		/* disable it */
		BPIE	&=	~BUTTON;
		IE2		|=	UCA0TXIE;
	}

	/* clear any interrupt flags */
	BPIFG	=	0x00;
}

void gpio_init(void) {
	BPDIR	&=	~BUTTON;	/* button as input */
	LPDIR	|=	LED;		/* LED as output */
	LPOUT	&=	~LED;		/* LED off */

	BPREN	|=	BUTTON;		/* enable resistor */
	BPOUT	|=	BUTTON;		/* as a pullup */
	BPIES	|=	BUTTON;		/* interrupt edge select as falling edge */
	/* we do not want to enable this interrupt just yet */
}
