#include "util_user.h"
#include "action.h"

inline void tap_code(uint16_t keycode) {
  register_code(keycode);
  unregister_code(keycode);
}

inline bool in_range(uint16_t keycode, uint16_t min, uint16_t max) {
  return keycode >= min && keycode <= max;
}
