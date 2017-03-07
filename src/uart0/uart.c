#include <stddef.h>
#include <msp430.h>

#include <interrupt.h>
#include <bitmanip.h>

#include "uart.h"


volatile char uart_buffer[34] = { '\r', '\n' };

ISR(USCIAB0TX_VECTOR) {
	static size_t position = 1;
	UCA0TXBUF = uart_buffer[position++];

	if(position == sizeof(uart_buffer)/sizeof(uart_buffer[0])) {
		/* We have reached the end of the buffer */
		position = 1;
		/* Disable the interrupt so that we do not send any more data */
		IE2 &= ~UCA0TXIFG;
		/* Enable receiving data. */
		IE2 |= UCA0RXIFG;
	}
}

ISR(USCIAB0RX_VECTOR) {
	static size_t position = 2;
	char c;

	/* capture the character that was received and store it in the buffer */
	c = UCA0RXBUF;
	uart_buffer[position++] = c;
	
	/* echo it back to the terminal */
	loop_until_bit_is_set(IFG2, UCA0TXIFG);
	UCA0TXBUF = c;
	
	if(position == sizeof(uart_buffer)/sizeof(uart_buffer[0])) {
		/* We have reached the end of the buffer */
		position = 2;
		/* Disable the interrupt so that we do not receive any more data */
		IE2	&= ~UCA0RXIFG;
		/* and enable the transmit interrupt */
		IE2 |= UCA0TXIFG;
	}
}

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

	/* enable receive interrupt */
	IE2			=	UCA0RXIFG;
}

int uart_putc(int i) {
	loop_until_bit_is_set(IFG2, UCA0TXIFG);

	UCA0TXBUF = i;
	return i;
}

size_t uart_write(const char* restrict buffer, size_t len) {
	size_t pos = 0;
	while(pos < len) {
		uart_putc(buffer[pos++]);
	}

	return len;
}
