#ifndef CONFIG_H__
#define CONFIG_H__

#include <msp430.h>

/* UART buffer size */
#define BUFFER_SIZE	63

/* board specific declarations */
#define LPORT		P1		/* GPIO port where the LED is at */
#define LED			BIT6	/* pin in which the LED is located at */

#define PPORT		P1		/* GPIO port where the `panic' led is at */
#define PANIC		BIT0	/* pin in which the `panic' led is at */

#define BPORT		P1		/* GPIO port where the button is at */
#define BUTTON		BIT3	/* pin in which the button is located at */

#define UPORT		P1		/* GPIO port where the UART is located at */
#define RXD			BIT1	/* pin which contains the UART receiver */
#define TXD			BIT2	/* pin which contains the UART transmitter */


/* no change should be made from here onwards */
/* force the preprocessor to expand a and b */
#define GLUE(a,b)	GLUE2(a,b)
/* the ## preprocessor token means concatenation, we join a and b on a single
 * statement
 */
#define GLUE2(a, b)	a##b

#define UPSEL		GLUE(UPORT, SEL)
#define UPSEL2		GLUE(UPORT, SEL2)

#define BPIE		GLUE(BPORT, IE)		/* interrupt enable register */
#define BPDIR		GLUE(BPORT, DIR)	/* direction register */
#define BPREN		GLUE(BPORT, REN)	/* resistor enable register */
#define BPIFG		GLUE(BPORT, IFG)	/* interrupt flag register */
#define BPIES		GLUE(BPORT, IES)	/* interrupt edge select register */
/* TI defines OUT in a header file... awesome */
#define BPOUT_		GLUE(BPORT, O)		/* workaround */
#define BPOUT		GLUE(BPOUT_, UT)	/* output register */

#define PPDIR		GLUE(PPORT, DIR)	/* direction register */
#define PPOUT_		GLUE(PPORT, O)		/* workaround */
#define PPOUT		GLUE(PPOUT_, UT)	/* output register */

#define LPDIR		GLUE(LPORT, DIR)	/* direction register */
#define LPOUT_		GLUE(LPORT, O)		/* workaround */
#define LPOUT		GLUE(LPOUT_, UT)	/* output register */

#endif	/* CONFIG_H__ */
