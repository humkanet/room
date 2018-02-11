#include <xc.h>
#include "clock.h"
#include "pre.h"


static struct {
	uint8_t  seconds;
} clock;


inline void clock_isr()
{
	if (IOCF(CLOCK_PORT, CLOCK_PIN)){
		IOCF(CLOCK_PORT, CLOCK_PIN) = 0;
		clock.seconds ++;
	}
}


inline void clock_init()
{
	IOCMD = 0;
	// Настраиваем пин
	TRIS(CLOCK_PORT, CLOCK_PIN) = 1;
	ANS(CLOCK_PORT, CLOCK_PIN)  = 0;
	// Прерывание по фронту
	IOCP(CLOCK_PORT, CLOCK_PIN) = 1;
	IOCN(CLOCK_PORT, CLOCK_PIN) = 0;
	IOCF(CLOCK_PORT, CLOCK_PIN) = 0;
	// Инициализация
	clock.seconds = 0;
}


inline uint8_t clock_sec()
{
	return clock.seconds;
}
