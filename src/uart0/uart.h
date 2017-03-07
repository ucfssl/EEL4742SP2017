#ifndef UART_H__
#define UART_H__

#include <stddef.h>

void uart_init(void);
int uart_putc(int i);
size_t uart_write(const char* restrict buffer, size_t len);

#endif
