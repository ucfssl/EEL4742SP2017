#include <stddef.h>
#include <msp430.h>

#include <bitmanip.h>

#include "uart.h"

void uart_init(void) {
	/* Set the reset flag in the USCI. This is automatically set by hardware on
	 * reset, but the manual recommends to do it anyway.
	 */
	UCA0CTL1	|=	UCSWRST;

	/* Configure USCI:
	 *	- Parity disabled		(UCPEN unset)
	 *	- Odd parity, ignored	(UCPAR unset)
	 *	- LSB first				(UCMSB unset)
	 *	- 8-bit data			(UC7BIT unset)
	 *	- One stop bit			(UCSPB unset)
	 *	- UART mode				(UCMODE_0)
	 *	- Asynchronous mode		(UCSYNC unset)
	 */
	UCA0CTL0	=	0;

	UCA0CTL1	|=	UCSSEL_2;	/* use SMCLK as clock source */

	/* Baud rate generator prescalers. See Table 15-5. */
	UCA0BR0		=	6;
	UCA0BR1		=	0;

	/* USCI_A0 Modulation Control Register setup */
	UCA0MCTL	=	(8 << 4)	/* UCBRF = 8 */
					| (0 << 1)	/* UCBRS = 0 */
					| UCOS16	/* oversampling enabled */
			;
	
	/* Release reset configuration */
	UCA0CTL1	&= ~UCSWRST;

	/* Pin function select. See Table 16 in slas735. */
	P1SEL		=	BIT2 | BIT1;
	P1SEL2		=	BIT2 | BIT1;
}

int uart_putc(int i) {
	/* wait until the transmit buffer is empty */
	loop_until_bit_is_set(IFG2, UCA0TXIFG);

	/* send the character */
	UCA0TXBUF = i;

	/* appease the POSIX gods emulating putc() */
	return i;
}

size_t uart_write(const char* restrict buffer, size_t len) {
	size_t pos = 0;
	
	/* while we are still in the buffer */
	while(pos < len) {
		/* send a character out and increment the position counter */
		uart_putc(buffer[pos++]);
	}

	/* appease the POSIX gods by returning the amount of bytes written */
	return len;
}
