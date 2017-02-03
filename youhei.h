#ifndef YOUHEI_H
#define YOUHEI_H

#include "ergodox.h"
#include "mousekey.h"

enum Layer {
	L_BASE = 0,
	L_SYMBOL,
	L_ARROW,
	L_NUM,
	L_FUNCTION,
};

// reset layer state
#define CLEAR M(0)

// tapping Layer
#define SYMB_T(kc) LT(L_SYMBOL,kc)
#define ARRW_T(kc) LT(L_ARROW, kc)
#define NUM_T(kc) LT(L_NUM, kc)
#define FN_T(kc) LT(L_FUNCTION, kc)

void tap_mod_mouse(keyrecord_t *record, uint8_t mod, uint8_t mouse) {
	if(record->event.pressed) {
		// if not tap
		if(record->tap.count <= 0 || record->tap.interrupted) {
			register_mods(mod);
		}
	}
	else {
		// if not tap
		if(record->tap.count <= 0 || record->tap.interrupted) {
			unregister_mods(mod);
		}
		else {
			mousekey_on(mouse);
			mousekey_send();
			mousekey_off(mouse);
			mousekey_send();
			record->tap.count = 0;
		}
	}
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
	switch(id) {
		case 0:
			tap_mod_mouse(record, MOD_LGUI, KC_BTN1);
			break;
		case 1:
			tap_mod_mouse(record, MOD_LALT, KC_BTN2);
			break;
		case 2:
			tap_mod_mouse(record, MOD_LCTL, KC_BTN3);
			break;
		default:
			break;
	}
};

const uint16_t PROGMEM fn_actions[] = {
	[0] = ACTION_FUNCTION_TAP(0),
	[1] = ACTION_FUNCTION_TAP(1),
	[2] = ACTION_FUNCTION_TAP(2),
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
	switch(id) {
		case 0:
			if(record->event.pressed) {
				layer_clear();
				clear_mods();
				clear_oneshot_mods();
			}
			break;
		default:
			break;
	}
	return MACRO_NONE;
};

#endif
