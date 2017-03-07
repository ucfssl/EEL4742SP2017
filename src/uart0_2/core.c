#include <msp430.h>

#include "dco.h"
#include "uart.h"

int main(void) {

	WDTCTL	=	WDTPW | WDTHOLD;

	/* calibrate the DCO */
	dco_calibrate();

	/* initialize the UART */
	uart_init();

	__bis_SR_register(CPUOFF | GIE);
}
