#include "ergodox_ez.h"
#include "util_user.h"
#include "action.h"
#include "process_simultaneous.h"

enum user_code {
	KC_P000 = SAFE_RANGE,
	CLEAR,
	// SH stands for Same Hand
	SH_TRNS,
};

enum Layer {
	L_BASE,
#ifdef ENABLE_STABLE_LAYER
	L_STABLE,
#endif
	L_FN,
	L_LSFT,
	L_RSFT,
};
#define LSFT_LS(kc) LS(L_LSFT, kc)
#define RSFT_LS(kc) LS(L_RSFT, kc)
#define FN_LS(kc) LS(L_FN, kc)

// click (left, middle, right)
#define KC_LCLK KC_BTN1
#define KC_MCLK KC_BTN3
#define KC_RCLK KC_BTN2

#ifdef ENABLE_STABLE_LAYER
#define STABLE DF(L_STABLE)
#define FN_T(kc) LT(L_FN, kc)
#else
#define STABLE CLEAR
#endif


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch(keycode) {
	case KC_P000:
		if(record->event.pressed) {
			for(uint8_t i=0; i<3; i++) {
				tap_code(KC_P0);
			}
		}
		return false;
	case CLEAR:
		if(record->event.pressed) {
			clear_mods();
			layer_clear();
		}
		return false;
	case SH_TRNS:
		if(record->simultaneous) {
			if(record->event.pressed) {
				tap_code(KC_3);
				uint16_t keycode = keymap_key_to_keycode(0, record->event.key);
				tap_code(keycode);
			}
		}
		else {
			uint32_t layers = layer_state | default_layer_state;
			keypos_t keypos = record->event.key;
			uint16_t keycode;
			int8_t i;
			for(i = L_LSFT - 1; i >= 0; i--) {
				if(layers & (1UL << i)) {
					keycode = keymap_key_to_keycode(i, keypos);
					if(keycode != KC_TRNS) break;
				}
			}
			if(i == -1) keycode = keymap_key_to_keycode(0, keypos);
			if(record->event.pressed) {
				register_code(keycode);
			}
			else {
				unregister_code(keycode);
			}
		}
		return false;
	default:
		break;
	}

#ifdef ENABLE_STABLE_LAYER
	uint8_t layer = biton32(default_layer_state);
	if(layer == L_STABLE) return true;
#endif
	return process_simultaneous(keycode, record);
}

void matrix_init_user(void) {
	ergodox_led_all_set(LED_BRIGHTNESS_LO);
	ergodox_led_all_off();
}

void matrix_scan_user(void) {
	matrix_scan_simultaneous();
}

void led_set_user(uint8_t usb_led) {
	// I usually set num lock on, I prefer led is on while in unusual state.
	// BUG: blink the led while suspend
	// if(usb_led & (1<<USB_LED_NUM_LOCK)) ergodox_right_led_1_off();
	// else ergodox_right_led_1_on();

	if(IS_LED_ON(usb_led, USB_LED_CAPS_LOCK)) ergodox_right_led_2_on();
	else ergodox_right_led_2_off();

	if(IS_LED_ON(usb_led, USB_LED_SCROLL_LOCK)) ergodox_right_led_3_on();
	else ergodox_right_led_3_off();
}

static bool is_shifted = false;
uint32_t layer_state_set_user(uint32_t state) {
	uint8_t layer = biton32(state);
	if(layer == L_LSFT || layer == L_RSFT) {
		if(!is_shifted) {
			is_shifted = true;
			register_code(KC_LSFT);
		}
	} else if(layer == L_BASE) {
		if(is_shifted) {
			is_shifted = false;
			unregister_code(KC_LSFT);
		}
	}
	return state;
}

#ifdef ENABLE_STABLE_LAYER
uint32_t default_layer_state_set_user(uint32_t state) {
	uint8_t layer = biton32(state);
	if(layer == L_STABLE) ergodox_right_led_1_on();
	else ergodox_right_led_1_off();
	return state;
}
#endif

bool has_simultaneous_priority_to_a(uint16_t keycode_a, uint16_t keycode_b) {
	if(is_simultaneous_layer(keycode_a)) {
		uint8_t layer_a = get_simultaneous_layer_from_keycode(keycode_a);
		if(layer_a == L_LSFT) return true;
		else if(layer_a == L_RSFT) {
			if(is_simultaneous_layer(keycode_b)) {
				uint8_t layer_b = get_simultaneous_layer_from_keycode(keycode_b);
				return layer_b != L_LSFT;
			}
			else return true;
		}
	}
	return false;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[L_BASE] = LAYOUT_ergodox(
		// left hand
		KC_PSCR, KC_3, KC_2, KC_1, KC_5, KC_4, KC_NO,
		KC_MHEN, KC_Q, KC_C, KC_L, KC_P, KC_J, KC_CAPS,
		KC_SLSH, KC_O, KC_S, KC_R, KC_N, KC_M,
		KC_MCLK, KC_X, KC_V, KC_MINS, KC_H, KC_B, KC_BSPC,
		KC_RCLK, KC_LCLK, LGUI_S(KC_GRV), LCTL_S(KC_BSLS), LALT_S(KC_EQL),
		// thumb
		KC_RCLK, KC_LCLK,
		KC_MCLK,
		LSFT_LS(KC_SPC), FN_LS(KC_ENT), KC_ESC,
		// right hand
		KC_NO, KC_8, KC_9, KC_0, KC_6, KC_7, STABLE,
		KC_INS, KC_K, KC_F, KC_U, KC_Y, KC_Z, KC_HENK,
		KC_G, KC_T, KC_E, KC_I, KC_A, KC_SCLN,
		KC_DEL, KC_W, KC_D, KC_COMM, KC_DOT, KC_QUOT, KC_LCLK,
		RALT_S(KC_LBRC), RCTL_S(KC_RBRC), RGUI_S(KC_PAUS), KC_MCLK, KC_RCLK,
		// thumb
		KC_WBAK, KC_WFWD,
		KC_WHOM,
		KC_KANA, FN_LS(KC_TAB), RSFT_LS(KC_SPC)
		),
#ifdef ENABLE_STABLE_LAYER
	[L_STABLE] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, LGUI_T(KC_GRV), LCTL_T(KC_BSLS), LALT_T(KC_EQL),
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		LSFT_T(KC_SPC), FN_T(KC_ENT), KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, DF(L_BASE),
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		RALT_T(KC_LBRC), RCTL_T(KC_RBRC), RGUI_T(KC_PAUS), KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, FN_T(KC_TAB), RSFT_T(KC_SPC)
		),
#endif
	[L_FN] = LAYOUT_ergodox(
		// left hand
		KC_NO, KC_F3, KC_F2, KC_F1, KC_F5, KC_F4, KC_NO,
		KC_NO, KC_NO, KC_P6, KC_P5, KC_P4, KC_NLCK, KC_TRNS,
		KC_NO, KC_PDOT, KC_P3, KC_P2, KC_P1, KC_P0,
		KC_NO, KC_NO, KC_P9, KC_P8, KC_P7, KC_P000, KC_TRNS,
		KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_VOLD, KC_VOLU,
		KC_MUTE,
		KC_TRNS, KC_TRNS, KC_NO,
		// right hand
		KC_NO, KC_F8, KC_F9, KC_F10, KC_F6, KC_F7, KC_NO,
		KC_TRNS, KC_NO, KC_LEFT, KC_RGHT, KC_F11, KC_F12, KC_NO,
		KC_HOME, KC_DOWN, KC_UP, KC_END, KC_APP, KC_NO,
		KC_TRNS, KC_NO, KC_PGDN, KC_PGUP, KC_SLCK, KC_NO, KC_NO,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO,
		// thumb
		KC_NO, KC_NO,
		KC_NO,
		KC_NO, KC_TRNS, KC_TRNS
		),
	[L_LSFT] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,
		KC_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, KC_TRNS,
		SH_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, SH_TRNS,
		KC_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, DF(L_BASE),
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
		),
	[L_RSFT] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, STABLE,
		KC_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, KC_TRNS,
		SH_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, SH_TRNS,
		KC_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, SH_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
		),
};
