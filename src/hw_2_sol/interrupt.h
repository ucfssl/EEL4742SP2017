/************************************************************
 * Interrupt declarations
 */
#ifndef INTERRUPT_H__
#define INTERRUPT_H__

#ifdef __GNUC__
/* We are using GCC, declare interrupts the GCC way */
#define ISR(vect__) \
	__attribute__((interrupt(vect__))) \
	void vect__##_isr(void); \
	void vect__##_isr(void)
#else
/* Section 6.10.3.4/3 of the C standard forbids us from having a preprocessor
 * directive inside a preprocessor directive. Luckily, C99 provides us a way
 * around that with the _Pragma directive. Some preprocessor hackery ensues in
 * order to declare interrupts the TI way.
 */
#define Str__(x) #x
#define ISR(vect__) \
	_Pragma( Str__(vector=vect__) ) \
	__interrupt void vect__##_isr(void)
#endif \

#endif	/* INTERRUPT_H__ */

/* vim:set ts=4 tw=79 syntax=c et: */
