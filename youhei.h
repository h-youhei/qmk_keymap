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
	L_CLICK,
};

// reset layer state
#define CLEAR KC_FN0

// tapping Layer
#define SYMB_T(kc) LT(L_SYMBOL,kc)
#define ARRW_T(kc) LT(L_ARROW, kc)
#define NUM_T(kc) LT(L_NUM, kc)
#define FN_T(kc) LT(L_FUNCTION, kc)
#define CLCK_T(kc) LT(L_CLICK, kc)

// tapping mod with special key
#define TRNS_T_LCLCK KC_FN1
#define TRNS_T_MCLCK KC_FN2
#define TRNS_T_RCLCK KC_FN3
#define TRNS_T_LPRN KC_FN4
#define TRNS_T_RPRN KC_FN5
#define TRNS_T_LALT KC_FN6

const macro_t *tap_mod_macro(keyrecord_t *record, uint8_t mod, const macro_t *macro) {
	if (record->event.pressed) {
		// not tap
		if (record->tap.count <= 0 || record->tap.interrupted) {
			register_mods(mod);
		}
	}
	else {
		// not tap
		if (record->tap.count <= 0 || record->tap.interrupted) {
			unregister_mods(mod);
		}
		// tap
		else {
			return macro;
		}
	}
	return MACRO_NONE;
};

const macro_t *tap_layer_macro(keyrecord_t *record, uint8_t layer, const macro_t *macro) {
	if (record->event.pressed) {
		// not tap
		if (record->tap.count <= 0 || record->tap.interrupted) {
			layer_on(layer);
		}
	}
	else {
		// not tap
		if (record->tap.count <= 0 || record->tap.interrupted) {
			layer_off(layer);
		}
		// tap
		else {
			return macro;
		}
	}
	return MACRO_NONE;
};

void tap_mousekey(uint8_t mouse) {
	mousekey_on(mouse);
	mousekey_send();
	mousekey_off(mouse);
	mousekey_send();
};

void tap_mod_mouse(keyrecord_t *record, uint8_t mod, uint8_t mouse) {
	if (record->event.pressed) {
		//  not tap
		if (record->tap.count <= 0 || record->tap.interrupted) {
			register_mods(mod);
		}
	}
	else {
		//  not tap
		if (record->tap.count <= 0 || record->tap.interrupted) {
			unregister_mods(mod);
		}
		// tap
		else {
			tap_mousekey(mouse);
		}
	}
};

void tap_layer_mouse(keyrecord_t *record, uint8_t layer, uint8_t mouse) {
	if (record->event.pressed) {
		// not tap
		if (record->tap.count <= 0 || record->tap.interrupted) {
			layer_on(layer);
		}
	}
	else {
		// not tap
		if (record->tap.count <= 0 || record->tap.interrupted) {
			layer_off(layer);
		}
		// tap
		else {
			tap_mousekey(mouse);
		}
	}
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
	switch(id) {
		case 0:
			if (record->event.pressed) {
				layer_clear();
				clear_mods();
				clear_oneshot_mods();
			}
			break;
		case 1:
			tap_layer_mouse(record, L_CLICK, KC_BTN1);
			break;
		case 2:
			tap_mod_mouse(record, MOD_LGUI, KC_BTN3);
			break;
		case 3:
			tap_mod_mouse(record, MOD_LALT, KC_BTN2);
			break;
		default:
			break;
	}
};

const uint16_t PROGMEM fn_actions[] = {
	[0] = ACTION_FUNCTION(0),
	[1] = ACTION_FUNCTION_TAP(1),
	[2] = ACTION_FUNCTION_TAP(2),
	[3] = ACTION_FUNCTION_TAP(3),
	[4] = ACTION_MACRO_TAP(0),
	[5] = ACTION_MACRO_TAP(1),
	[6] = ACTION_MACRO_TAP(2),
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
	switch(id) {
		case 0:
			return tap_mod_macro(record, MOD_LSFT, MACRO(D(LSFT), T(9), U(LSFT), END));
			break;
		case 1:
			return tap_layer_macro(record, L_CLICK, MACRO(D(LSFT), T(0), U(LSFT), END));
			break;
		case 2:
			return tap_layer_macro(record, L_CLICK, MACRO(D(LALT), T(NO), U(LALT), END));
		default:
			break;
	}
	return MACRO_NONE;
};

void matrix_init_user(void) {
	ergodox_led_all_set(LED_BRIGHTNESS_LO);
};

void matrix_scan_user(void) {
	ergodox_led_all_off();

	if (host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK)) {
		ergodox_right_led_2_on();
	}
};

#endif
