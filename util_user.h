#ifndef UTIL_USER_H
#define UTIL_USER_H

#include <stdint.h>
#include <stdbool.h>
#include "keyboard.h"

void tap_code(uint16_t keycode);
bool in_range(uint16_t keycode, uint16_t min, uint16_t max);
uint16_t get_keycode_from_keypos(keypos_t keypos);
uint8_t mod_for_send(uint8_t mod);

#endif
