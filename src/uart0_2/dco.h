#ifndef DCO_H__
#define DCO_H__

#ifdef __GNUC__
/* If we are compiling with gcc, add this function to the constructors array.
 * The runtime will automatically call it for us. Otherwise, we must explicitly
 * call this function from main().
 */
__attribute__((constructor))
#endif
void dco_calibrate(void);

#endif	/* DCO_H__ */
