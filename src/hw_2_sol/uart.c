#include <stddef.h>
#include <msp430.h>

#include "bitmanip.h"
#include "config.h"
#include "interrupt.h"
#include "morse.h"
#include "uart.h"

volatile char uart_buffer[BUFFER_SIZE];

/* Transmit Interrupt */
ISR(USCIAB0TX_VECTOR) {
	static size_t pattern_position = 2;
	static size_t msg_position;

	static const char morse_out[] = {'.', '-'};
	const uint8_t* pattern;
	char current_char;

	if(msg_position == BUFFER_SIZE ||
			(current_char = uart_buffer[msg_position]) == '\r') {
		/* We have reached the end of the buffer, reset the print position */
		msg_position = 0;
		/* Disable the interrupt so that we do not send any more data */
		IE2		&=	~UCA0TXIE;
		/* There is one edge case here: we are done blinking the LED and the
		 * timer stopped running but we are still transmitting through the
		 * UART. We can not enable the button interrupt if this is the case so
		 * we perform a check.
		 */
		if(bit_is_set(TACCTL0, CCIE)) {
			/* if the timer is still running, enable the button interrupt */
			BPIE	|=	BUTTON;
		}
	} else if(current_char == ' ') {
		/* if we have a space, send a space */
		UCA0TXBUF = ' ';
		/* and increment the pattern position */
		msg_position++;
		return;
	} else if(current_char >= 'A' && current_char <= 'Z') {
		/* we have a letter */
		pattern = morse[current_char - 'A'];
	} else {
		/* we have other stuff on the buffer, ignore it */
		msg_position++;
		/* and bail out */
		return;
	}

	if(pattern_position >= pattern[0]) {
		/* if we have finished transmitting the letter, send a space */
		UCA0TXBUF = ' ';
		/* reset the pattern position */
		pattern_position = 2;
		/* set up to process the next character in the message */
		msg_position++;
		/* and bail out */
		return;
	}

	/* Barrage of tricks ensue. Please make sure you read the comments to
	 * understand what is going on.
	 */

	/* The trick in this code is that a dot is encoded as a single 1, and the
	 * value following it is 0, whereas a dash is encoded as 1,1,1 followed by
	 * a 0. We do not look at the first character in the pattern, but the
	 * second one. If we are looking at a dot, we see a 0, if we are looking at
	 * a dash, we see a 1. We utilize this to our advantage to simplify the
	 * logic.
	 *
	 * A dash is encoded as
	 * 1, 1, 1, 0, 1, ?
	 *    ^
	 * We are looking at the character with the ^. Since it is a 1, we
	 * conclude that we have a dash and send out the proper character.
	 *
	 * A dot is encoded as
	 * 1, 0, 1, ?
	 *    ^
	 * We are looking at the character with the ^. Since it has a 0, we
	 * conclude that we have a dot and send out the proper character.
	 *
	 * Indexing into an array to pick the character we want to send out avoids
	 * us having to do any branching.
	 */
	UCA0TXBUF = morse_out[pattern[pattern_position]];
	/* If we are looking at a 0 [dot], we need to advance by 2 to get to the next
	 * item in the pattern to test. If we are looking at a 1 [dash], we need to
	 * advance by 4 to get to the next item in the pattern to test. We make the
	 * observation that 2 == 2 << 0, and that 4 == 2 << 1. This also saves us
	 * from having to do any branching. The problem is that MSP430 does not
	 * have a way to perform variable bitshifts. MSP430X can translate the
	 * expression E1 << E2 into a single instruction only if E2 is a constant.
	 * The case where E2 is variable must be emulated with a loop construct. It
	 * is cheaper to just branch then.
	 */
	pattern_position += pattern[pattern_position] ? 4 : 2;
}

/* Receive Interrupt */
ISR(USCIAB0RX_VECTOR) {
	static size_t position;
	char c;

	/* capture the character that was received and store it in the buffer */
	c = UCA0RXBUF;

	/* Ok, my C library pulls in locale information for toupper(). We avoid
	 * that insanity with more insanity.
	 */
	uart_buffer[position++] = (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
	
	/* echo it back to the terminal */
	loop_until_bit_is_set(IFG2, UCA0TXIFG);
	UCA0TXBUF = c;
	
	if(c == '\r' || position == BUFFER_SIZE) {
		/* We have reached the end of the buffer, reset the store position  */
		position = 0;
		/* Disable the receive interrupt so that we do not receive any more data */
		IE2		&= ~UCA0RXIE;
		/* reset the timer */
		TA0R	=	0;
		/* and enable its interrupt */
		TACCTL0	=	CCIE;
		/* clear any pending flags on the button press */
		BPIFG	&=	~BUTTON;
		/* also, enable the button interrupt */
		BPIE	|=	BUTTON;
	}
}

void uart0_init(void) {
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
	UPSEL		=	TXD | RXD;
	UPSEL2		=	TXD | RXD;

	/* enable receive interrupt */
	IE2			=	UCA0RXIE;
}
