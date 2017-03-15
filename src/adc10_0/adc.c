#include <stddef.h>
#include <stdint.h>
#include <msp430.h>

#include <bitmanip.h>

#include "dco.h"
#include "uart.h"

char* to_hex(char* buffer, uint16_t value);

__attribute__((noreturn))
void main(void) {
	uint16_t adc_read;
	char buffer[6] = { [4] = '\r', [5] = '\n' };

	WDTCTL	=	WDTPW | WDTHOLD;	/* disable WDT+ module */

	/* calibrate the DCO */
	dco_calibrate();
	/* initialize the UART */
	uart_init();

	/* before configuring the ADC10 module, ENC must be cleared */
	ADC10CTL0	&=	ENC;

	ADC10CTL0	=	SREF_1			/* V_ref+ and V_ss as references */
					| ADC10SHT_3	/* sample for 16 ADC clock cycles */
					| REFON			/* reference generator on */
					| ADC10ON;		/* turn on the ADC */
	/* sample period for the temperature sensor must be higher than 30us */
	ADC10CTL1	=	INCH_10			/* temperature sensor channel */
					| ADC10DIV_2	/* divide source clock by 2 */
					| ADC10SSEL_3;	/* use SMCLK */
	/* Using SMCLK at 1MHz, dividing its frequency by 2, then sampling for 16
	 * cycles gives us a sample time of 32us. This satisfies the sampling
	 * conditions.
	 */
	
	while(1) {
		ADC10CTL0	|=	ENC			/* enable conversion */
						| ADC10SC;	/* start conversion */
		/* wait for conversion to finish */
		loop_until_bit_is_set(ADC10CTL0, ADC10IFG);
		
		/* grab value */
		adc_read = ADC10MEM;
		
		/* convert it to hexadecimal */
		to_hex(buffer, adc_read);

		/* and print it */
		uart_write(buffer, 6);
		__delay_cycles(100000);
	}
}

char* to_hex(char* buffer, uint16_t value) {
	char t;
	for(size_t i = 0; i < 4; i++) {
		t = value & 0xf;
		value >>= 4;
		buffer[3 - i] = (t < 10) ? (t + '0') : (t + 'a' - 10);
	}
	return buffer;
}
