#include <stdint.h>

#include <msp430.h>

#define TOP     7800
#define BOTTOM  100
#define LED1    BIT0

#ifdef __GNUC__
/* GCC exposes the identifier __GNUC__. We check if this symbol is exposed and
 * if so, we declare interrupts the GCC way
 */
__attribute__((interrupt(TIMER0_A0_VECTOR)))
#else
/* Otherwise, we are using TI's compiler. Declare interrupts the TI way */
#pragma vector=TIMER0_A0_VECTOR
__interrupt
#endif
void timer0_isr(void) {
    /* We declare a variable with permanent store [static storage class
     * specifier] and initialize it to 100. Remember that variables with
     * permanent store that have been explicitly initialized get allocated in
     * the .data section and are initialized by the runtime at program startup.
     */
    static uint16_t compare = BOTTOM;
    
    /* We also declare another variable with permanent store, but we do not
     * initialize it. Remember that variables that have permanent store but are
     * not explicitly initialized go into the .bss section and are initialized
     * by the runtime to 0.
     */
    static uint8_t count_down;
    
    if(count_down) {
        /* if we are counting down, decrease the counter */
        compare -= 4;
        if(compare <= BOTTOM) {
            /* if we have reached the bottom, start counting up */
            count_down = 0;
        }
    } else {
        /* otherwise, we are counting up */
        compare += 4;
        if(compare >= TOP) {
            /* if we have reached the top, start counting down */
            count_down = 1;
        }
    }

    TACCR1 = compare;
}

int main(void) {
	/* WDT+ Module Setup */
	WDTCTL 	=	WDTPW | 	/* writing to the WDT+ registers
							 * requires the proper password */
				WDTHOLD;	/* stop the WDT+ module from counting */

	/* GPIO setup */
	P1DIR	=	0xff;		/* All pins as output on P1 */
	P1OUT	=	0x00;		/* Initialize all pins as low */
    P1SEL   |=   BIT6;      /* configure pin as Timer_A0.Compare1 output */
    P2SEL2  &=  ~BIT6;      /* see Table 19 in slas735j for pin functions */

    /* For this demonstration to work, we need to speed up the clock. We need
     * to use the Digitally Controlled Oscillator (DCO). Before using the DCO,
     * we need to calibrate it.
     */
    if(CALBC1_8MHZ == 0xFF) {
        /* The calibration data is in info memory. This is a factory configured
         * portion of flash. If it got erased, panic. Our panic function is
         * turning on the red LED and leaving it solid. */
        P1OUT = LED1;
        /* there is nothing else we can do, so just turn off the CPU */
        __bis_SR_register(CPUOFF);
    }

    DCOCTL  =   0;              /* start new DCO configuration */
    BCSCTL1 =   CALBC1_8MHZ;    /* select the RSEL value from calibration */
    DCOCTL  =   CALDCO_8MHZ;    /* and select step and modulation for DCO */

	
	/* Timer_A Setup -- details on Chapter 12 of slau144j */
	/* capture/compare control register 0 */
	TACCTL0	=	CCIE;		/* enable capture/compare interrupt */
	/* capture/compare value register 0 */
	TACCR0	=	8000;		/* set count top */
	/* timer control */
	TACTL	=	TASSEL_2 |	/* clock source selection - use SMCLK */
				MC_1 |		/* up mode - count to TACCR0, then reset */
				ID_0;		/* clock divider - divide SMCLK by 1 */
    /* capture/compare value register 1 */
    TACCR1  =   BOTTOM;
    /* capture/compare control register 1 configuration for PWM */
    TACCTL1 =   OUTMOD_3;   /* Timer in set/reset mode, set the output when
                             * reaching TACCR1 and reset it when reaching
                             * TACCR0. This generates our PWM wave.
                             */ 

	
	/* shut off CPU and enable interrupts */
	__bis_SR_register(CPUOFF | GIE);
}

/* vim:set ts=4 tw=79 syntax=c et: */
