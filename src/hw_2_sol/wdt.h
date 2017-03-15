#ifndef WDT_H__
#define WDT_H__

#ifdef __GNUC__
/* first user constructor available */
__attribute__((constructor(101)))
#else
/* for some reason, TI does not implement arguments for this attribute */
__attribute__((constructor))
#endif
void wdt_disable(void);

#endif	/* WDT_H__ */
