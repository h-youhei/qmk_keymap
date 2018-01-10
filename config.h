#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include QMK_KEYBOARD_CONFIG_H

/* #define ENABLE_STABLE_LAYER */

#define TAPPING_TERM 200

#define PREVENT_STUCK_MODIFIERS

#define PERMISSIVE_HOLD

#define SIMULTANEOUS_WAIT_TERM 80
#define SIMULTANEOUS_ALLOW_PERCENTAGE 70

#define NO_ACTION_FUNCTION
#define NO_ACTION_MACRO
#define NO_ACTION_ONESHOT
#ifndef ENABLE_STABLE_LAYER
#define NO_ACTION_TAPPING
#endif

#endif