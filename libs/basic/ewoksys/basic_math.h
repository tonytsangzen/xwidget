#ifndef BASIC_MATH_H
#define BASIC_MATH_H

#include <ewoksys/ewokdef.h>

#ifdef __cplusplus 
extern "C" {
#endif

uint32_t random_u32(void);
uint32_t random_to(uint32_t to);
uint32_t abs_32(int32_t v);

#define M_PI 3.14159265358979323846264338327950288

#ifdef __cplusplus
} 
#endif

#endif
