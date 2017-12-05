#ifndef YOUHEI_H
#define YOUHEI_H

#include "ergodox.h"
#include "mousekey.h"

enum Layer {
	L_BASE,
	L_FN,
};
#define FN_T(kc) LT(L_FN, kc)

// click (left, middle, right)
#define KC_LCLK KC_BTN1
#define KC_MCLK KC_BTN3
#define KC_RCLK KC_BTN2

const uint16_t PROGMEM fn_actions[] = {
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
	switch(id) {
		default:
			break;
	}
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
		default:
			break;
	}
	return true;
};

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
};

#endif
