#include <msp430.h>

#include "gpio.h"
#include "timer.h"
#include "uart.h"

int main(void) {
	gpio_init();
	uart0_init();
	timera0_init();

	__bis_SR_register(CPUOFF | GIE);

	while(1);
}
