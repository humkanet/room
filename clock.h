#ifndef _CLOCK_H
#define	_CLOCK_H

#include <stdint.h>


#define CLOCK_PORT  A
#define CLOCK_PIN   5


inline void     clock_isr();

inline void     clock_init();
inline uint8_t  clock_sec();

#endif
