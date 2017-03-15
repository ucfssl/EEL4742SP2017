#include <msp430.h>

#include "wdt.h"

void wdt_disable(void) {
	WDTCTL	=	WDTPW | WDTHOLD;
}
