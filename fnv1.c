#include "fnv1.h"

#define FNV_32_PRIME  0x01000193U;


uint32_t fnv1_hash(uint8_t *data, uint8_t len)
{
	uint32_t hash = 0x811c9dc5;
	while (len--){
		hash ^= *data++;
		hash *= FNV_32_PRIME;
	}
	return hash;
}
