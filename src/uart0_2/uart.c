#include <ctype.h>
#include <stddef.h>
#include <msp430.h>

/* some extra header files that are part of my code libraries */
#include <interrupt.h>
#include <bitmanip.h>

#include "uart.h"


volatile char uart_buffer[34] = { '\r', '\n' };

/* Transmit Interrupt */
ISR(USCIAB0TX_VECTOR) {
	/* This is the backbone of a ``non-blocking write''. We can utilize a
	 * slightly modified version of this interrupt to implement a write()
	 * function that immediately returns. The implementation of write() must,
	 * however account for possible race conditions between the buffer to write
	 * and the amount of data the interrupt has transmitted.
	 */
	static size_t position = 1;
	/* send the next character and increment the position counter */
	char c = uart_buffer[position++];
	if(isupper(c)) {
		UCA0TXBUF = tolower(c);
	} else {
		UCA0TXBUF = toupper(c);
	}

	if(position == sizeof(uart_buffer)/sizeof(uart_buffer[0])) {
		/* We have reached the end of the buffer, reset the print position */
		position = 1;
		/* Disable the interrupt so that we do not send any more data */
		IE2 &= ~UCA0TXIFG;
		/* Enable receiving data. */
		IE2 |= UCA0RXIFG;
	}
}

/* Receive Interrupt */
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
		/* We have reached the end of the buffer, reset the store position  */
		position = 2;
		/* Disable the receive interrupt so that we do not receive any more data */
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
	/* wait until the transmit buffer is empty */
	loop_until_bit_is_set(IFG2, UCA0TXIFG);

	/* send the character */
	UCA0TXBUF = i;

	/* appease the POSIX gods emulating putc() */
	return i;
}

size_t uart_write(const char* restrict buffer, size_t len) {
	/* This function implements what we call a ``blocking write'', since we
	 * never return from it until all the bytes that we set out to write on the
	 * UART are actually written. This simplifies our program, but introduces
	 * potentially unnecessary delays.
	 */
	size_t pos = 0;
	
	/* while we are still in the buffer */
	while(pos < len) {
		/* send a character out and increment the position counter */
		uart_putc(buffer[pos++]);
	}

	/* appease the POSIX gods by returning the amount of bytes written */
	return len;
}
