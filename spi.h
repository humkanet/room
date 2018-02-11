#ifndef _SPI_H
#define	_SPI_H

#include <stdint.h>


void     spi_init();
uint8_t  spi_xchg(uint8_t data);
void     spi_off();

#endif
