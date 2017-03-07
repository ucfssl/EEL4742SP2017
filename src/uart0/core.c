#include <msp430.h>

#include "uart.h"

#ifndef __GNUC__
/* if we are not using gcc, we need these headers */
#include "wdt.h"
#include "dco.h"
#endif

#ifdef __GNUC__
__attribute__((noreturn))
void
#else
int
#endif
main(void) {

#	ifndef __GNUC__
	/* If we are compiling with gcc, the symbol __GNUC__ is defined and we do
	 * not run any of these functions. Since we added these functions to the
	 * constructors array, the runtime takes care of that for us. If we are not
	 * using gcc, we must call these functions manually
	 */
	wdt_disable();
	dco_calibrate();
#	endif

	uart_init();

	__bis_SR_register(CPUOFF | GIE);
	while(1) {
		;
	}
}
