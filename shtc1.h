#ifndef _SHTC1_H
#define	_SHTC1_H

#include <stdint.h>
#include <stdbool.h>


#define SHTC1_ADDR            0x70
#define SHTC1_MEASURE_HPM_US  14400
#define SHTC1_MEASURE_LPM_US  940
#define SHTC1_RESET_US        230

bool      shtc1_measure();
uint16_t  shtc1_read_id();
int16_t   shtc1_temperature();
uint16_t  shtc1_humidity();

#endif
