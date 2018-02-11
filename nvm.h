#ifndef _NVM_H
#define	_NVM_H

#include <stdint.h>


void  nvm_read(uint16_t addr, uint8_t *data, uint8_t len);

#endif
