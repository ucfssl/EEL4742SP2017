#ifndef UART_H__
#define UART_H__

#include <stddef.h>

void uart0_init(void);
int uart0_putc(int i);
size_t uart0_write(const char* restrict buffer, size_t len);

#endif
