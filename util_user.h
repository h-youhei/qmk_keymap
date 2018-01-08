#ifndef UTIL_USER_H
#define UTIL_USER_H

#include <stdint.h>
#include <stdbool.h>

void tap_code(uint16_t keycode);
bool in_range(uint16_t keycode, uint16_t min, uint16_t max);

#endif
