#include <msp430.h>

#include "config.h"
#include "dco.h"

void dco_calibrate(void) {
	/* We must calibrate the DCO for proper UART operation. Otherwise, the UART
	 * BAUD computation will be wrong. The DCO is covered in Chapter 5 of the
	 * family datasheet (slau144).
	 */
	
	if(CALBC1_1MHZ == 0xff) {
		/* The calibration data is in info memory. This is a factory configured
		 * portion of flash. If it got erased, we panic. Our panic function
		 * turns on the red LED and leaves it solid. We also shut off the CPU
		 * since there is nothing else to do at this point.
		 */
		PPDIR = PANIC;
		PPOUT = PANIC;
		__bis_SR_register(CPUOFF);
	}

	DCOCTL	=	0;				/* new DCO configuration */
	BCSCTL1	=	CALBC1_1MHZ;	/* select the RSEL value from calibration */
	DCOCTL	=	CALDCO_1MHZ;	/* and select step and modulation for DCO */
}
