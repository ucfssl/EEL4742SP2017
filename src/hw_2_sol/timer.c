#include <stddef.h>

#include <msp430.h>


#include "config.h"
#include "interrupt.h"
#include "morse.h"
#include "timer.h"

extern volatile char uart_buffer[BUFFER_SIZE];

/* timer capture compare 0 interrupt */
ISR(TIMER0_A0_VECTOR) {
	static size_t msg_position;
	static size_t pattern_position = 1;
	
	const uint8_t* pattern;
	char current_char;
	
	if(msg_position == BUFFER_SIZE ||
			(current_char = uart_buffer[msg_position]) == '\r') {
		/* A few things are going on in the above line. First, we check whether
		 * we have reached the end of the buffer. If we have, then the boolean
		 * or operator || is short circuited and the rest of the statement is
		 * not evaluated. This avoids a potential out of bounds read in
		 * uart_buffer[]. If not, we get a character from uart_buffer[], and
		 * store it in current_char. The trick here is that the assignment
		 * operator also ``returns'' a value, the value of the assignment. We
		 * check that ``return'' and compare it against our message terminator
		 * character '\r'. If the comparison yields true, then we are at the
		 * end of the message and the following code executes. In the words of
		 * the great guru Dennis Ritchie: ``This will not be on the exam''. In
		 * my words: I expect bricks to be thrown at my window.
		 */

		/* end of message reached */
		msg_position = 0;		/* reset message position */
		TACCTL0	&=	~CCIE;		/* disable the timer interrupt */
		/* We may have received a character while sending morse code. This
		 * means an interrupt may be pending. We ``flush'' the character by
		 * clearing the interrupt flag.
		 */
		IFG2	&=	~UCA0RXIFG;
		IE2		|=	UCA0RXIE;	/* enable receiving again */
		/* disable the button interrupt */
		BPIE	&=	~BUTTON;
		/* and clear any pending flags */
		BPIFG	&=	~BUTTON;
		/* then bail out */
		return;
	} else if(current_char == ' ') {
		/* we have a space */
		pattern = space;
	} else if(current_char >= 'A' && current_char <= 'Z') {
		/* can't use isalpha() because i pull in internationalization stuff */
		/* we have a letter, which we stored as an uppercase */
		pattern = morse[current_char - 'A'];
	} else {
		/* we have some other nonsense, ignore it */
		msg_position++;
		/* and bail out */
		return;
	}

	/* grab the patter */
	if(pattern[pattern_position++]) {
		LPOUT	|=	LED;
	} else {
		LPOUT	&=	~LED;
	}

	/* if we are at the end of the pattern */
	if(pattern_position == pattern[0]) {
		/* go to the next position */
		msg_position++;
		/* and reset the pattern */
		pattern_position = 1;
	}
}

void timera0_init(void) {
	/* Set up timer in up mode. SMCLK at 1MHz, /8 prescaler. */
	TACCR0	=	31249;
	TACTL	=	TASSEL_2 |
				MC_1 |
				ID_3;
	/* We do not want to enable interrupts on the timer just yet. */
}
