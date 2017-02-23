#ifndef YOUHEI_H
#define YOUHEI_H

#include "ergodox.h"
#include "mousekey.h"

enum Layer {
	L_BASE,
	L_LREPEAT,
	L_RREPEAT,
	L_SIGN,
	L_DIRECTION,
	L_NUMBER,
	L_FUNCTION,
};

/*
enum Macro {
	M_NONE,
};

enum Function {
	F_GUI_T_MCLK,
};

enum Action {
	A_GUI_T_MCLK,
};
*/

enum Keycode_User {
	CLEAR = SAFE_RANGE,
};

// oneshot modifier
#define OS_SFT OSM(MOD_LSFT)

// tapping Layer
#define SIGN_T(kc) LT(L_SIGN,kc)
#define DIR_T(kc) LT(L_DIRECTION, kc)
#define NUM_T(kc) LT(L_NUMBER, kc)
#define FN_T(kc) LT(L_FUNCTION, kc)
#define LRPT_T(kc) LT(L_LREPEAT, kc)
#define RRPT_T(kc) LT(L_RREPEAT, kc)

// click (left, middle, right)
#define KC_LCLK KC_BTN1
#define KC_MCLK KC_BTN3
#define KC_RCLK KC_BTN2

// aliases more meaningful for me
#define KC_MULTI RALT(KC_NO)
#define KC_CMPS KC_MULTI
#define KC_TOGGLE_INPUT_METHOD KC_HENK
#define KC_TGIM KC_TOGGLE_INPUT_METHOD

// tapping key with special key
//#define GUI_T_MCLK F(A_GUI_T_MCLK)

/*
bool is_hold(keyrecord_t *record) {
	return (record->tap.count <= 0 || record->tap.interrupted);
}

const macro_t *tap_macro_hold_mod(keyrecord_t *record, uint8_t mod, const macro_t *macro) {
	if (record->event.pressed) {
		if (is_hold(record)) {
			register_mods(mod);
		}
	}
	else {
		if (is_hold(record)) {
			unregister_mods(mod);
		}
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

void tap_mouse_hold_mod(keyrecord_t *record, uint8_t mod, uint8_t mouse) {
	if (record->event.pressed) {
		if (is_hold(record)) {
			register_mods(mod);
		}
	}
	else {
		if (is_hold(record)) {
			unregister_mods(mod);
		}
		else {
			tap_mousekey(mouse);
		}
	}
};

const macro_t *tap_macro_hold_layer(keyrecord_t *record, uint8_t layer, const macro_t *macro) {
	if (record->event.pressed) {
		if (is_hold(record)) {
			layer_on(layer);
		}
	}
	else {
		if (is_hold(record)) {
			layer_off(layer);
		}
		else {
			return macro;
		}
	}
	return MACRO_NONE;
};

void tap_mouse_hold_layer(keyrecord_t *record, uint8_t layer, uint8_t mouse) {
	if (record->event.pressed) {
		if (is_hold(record)) {
			layer_on(layer);
		}
	}
	else {
		if (is_hold(record)) {
			layer_off(layer);
		}
		else {
			tap_mousekey(mouse);
		}
	}
};
*/

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
	switch(id) {
		/*
		case F_GUI_T_MCLK:
			tap_mouse_hold_mod(record, MOD_LGUI, KC_MCLK);
			break;
		*/
		default:
			break;
	}
};

const uint16_t PROGMEM fn_actions[] = {
	//[A_GUI_T_MCLK] = ACTION_FUNCTION_TAP(F_GUI_T_MCLK),
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
	switch(id) {
		default:
			break;
	}
	return MACRO_NONE;
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch(keycode) {
		case CLEAR:
			if (record->event.pressed) {
				reset_oneshot_layer();
				layer_clear();
				clear_mods();
				clear_oneshot_mods();
			}
			return false;
			break;
		default:
			break;
	}
	return true;
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
