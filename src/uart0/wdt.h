#ifndef WDT_H__
#define WDT_H__

#ifdef __GNUC__
/* If we are compiling with gcc, place the address of this function as the
 * first user entry in the constructors array. The runtime will pick up the
 * address and automatically run the function for us as early as possible.
 */
__attribute__((constructor(101)))
#endif
void wdt_disable(void);

#endif	/* WDT_H__ */
