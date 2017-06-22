#ifndef YOUHEI_H
#define YOUHEI_H

#include "ergodox.h"
#include "mousekey.h"

enum Layer {
	L_BASE,
	L_FN,
};

/*
enum Action {
};


enum Keycode_User {
	CLEAR = SAFE_RANGE,
};
*/

// tapping Layer
//#define DIR_T(kc) LT(L_DIR, kc)
#define FN_T(kc) LT(L_FN, kc)

// click (left, middle, right)
#define KC_LCLK KC_BTN1
#define KC_MCLK KC_BTN3
#define KC_RCLK KC_BTN2

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

void tap_mousekey(uint8_t mouse) {
	mousekey_on(mouse);
	mousekey_send();
	mousekey_off(mouse);
	mousekey_send();
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
	switch(id) {
		case GUI_T_RCLK:
			tap_mouse_hold_mod(record, MOD_LGUI, KC_RCLK);
			break;
		default:
			break;
	}
};

const uint16_t PROGMEM fn_actions[] = {
	[GUI_T_RCLK] = ACTION_FUNCTION_TAP(GUI_T_RCLK),
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
*/

void matrix_init_user(void) {
	ergodox_led_all_off();
};

void matrix_scan_user(void) {
	uint32_t state = host_keyboard_leds();

	if (state & (1 << USB_LED_CAPS_LOCK)) {
		ergodox_right_led_2_set(LED_BRIGHTNESS_LO);
		ergodox_right_led_2_on();
	}
	else {
		ergodox_right_led_2_off();
	}

//	switch(biton32(layer_state)) {
//		case L_PAD:
//			ergodox_right_led_3_set(LED_BRIGHTNESS_LO);
//			ergodox_right_led_3_on();
//			break;
//		default:
//			ergodox_right_led_3_off();
//		break;
//	}
};

#endif
