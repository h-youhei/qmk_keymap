#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include QMK_KEYBOARD_CONFIG_H

#undef TAPPING_TERM
#define TAPPING_TERM 250

#define NO_ACTION_FUNCTION
#define NO_ACTION_MACRO
#define NO_ACTION_ONESHOT

// #define ENABLE_STABLE_LAYER
// #define NO_JAPANESE

#define TAPPING_FORCE_HOLD
#define PERMISSIVE_HOLD
#define STRICT_LAYER_RELEASE

#define MOUSEKEY_WHEEL_INTERMAL 20
#define MOUSEKEY_WHEEL_MAX_SPEED 6
#define MOUSEKEY_WHEEL_TIME_TO_MAX 40
// #define MK_3_SPEED
// #define MK_W_INTERVAL_1 70

#define COMMIT_AT_PUNCTUATION
#endif
