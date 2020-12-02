#ifndef HAL_SHA2_H
#define HAL_SHA2_H

#include <stdint.h>
#include <stddef.h>

void hal_mul128(uint32_t r[8], const uint32_t a[4], const uint32_t b[4]);
void hal_mul256(uint32_t r[16], const uint32_t a[8], const uint32_t b[8]);

#endif