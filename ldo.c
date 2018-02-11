#include <xc.h>
#include "ldo.h"
#include "pre.h"


inline void ldo_init()
{
	LAT(LDO_PORT, LDO_PIN)  = 0;
	TRIS(LDO_PORT, LDO_PIN) = 0;
}


inline void ldo_on()
{
	LAT(LDO_PORT, LDO_PIN) = 1;
}


inline void ldo_off()
{
	LAT(LDO_PORT, LDO_PIN) = 0;
}
