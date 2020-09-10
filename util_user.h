#ifndef UTIL_USER_H
#define UTIL_USER_H

#include <stdint.h>
#include <stdbool.h>
#include "keyboard.h"

bool in_range(uint16_t keycode, uint16_t min, uint16_t max);
uint16_t get_keycode_from_keypos(keypos_t keypos);
bool is_shifting_but_other_mod(void);
void tap_shifted_code(uint16_t keycode);
void tap_code_unmods(uint16_t keycode);
uint8_t mod_for_send(uint8_t mod);

#endif
