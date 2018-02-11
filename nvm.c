#include <xc.h>
#include "nvm.h"


void nvm_read(uint16_t addr, uint8_t *data, uint8_t len)
{
#ifdef NVMMD_bit
	NVMMD  = 0;
#endif
	NVMCON1bits.NVMREGS = 1;
	NVMADR = addr;
	while(len--){
		NVMCON1bits.RD = 1;
		*data++        = NVMDAT;
		NVMADR ++;
	}
	NVMCON1bits.NVMREGS = 0;
#ifdef NVMMD_bit
	NVMMD  = 1;
#endif
}
