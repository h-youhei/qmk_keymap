#include "ergodox_ez.h"
#include "util_user.h"
#include "action.h"
#include "keymap_jp.h"

#include "my.h"

#ifndef NO_JAPANESE
#include "kana.h"
#include "kana_chord.h"
#include "ime.h"
#endif

#define SFT_LT(kc) LT(L_SHIFT, kc)
#define FN_T(kc) LT(L_FN, kc)

// click (left, middle, right)
#define KC_LCLK KC_BTN1
#define KC_MCLK KC_BTN3
#define KC_RCLK KC_BTN2

#ifdef ENABLE_STABLE_LAYER
#define STABLE DF(L_STABLE)
#else
#define STABLE CLEAR
#endif

void matrix_scan_user(void) {
#ifndef NO_JAPANESE
	// matrix_scan_kana();
#endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	keyevent_t event = record->event;
#ifndef NO_JAPANESE
	if(!process_ime(keycode, record)) return false;
#endif
	switch(keycode) {
	case CLEAR:
		if(event.pressed) {
			clear_mods();
			layer_clear();
#ifndef NO_JAPANESE
			reset_ime();
#endif
		}
		return false;
#ifndef NO_JAPANESE
	case LALT_T(KC_COMM):
		// use default process other than KANA layer
		if(!is_default_layer_kana()) return true;
		// use default process for mod
		if(record->tap.count > 0) {
			return process_ime(KANA_COMM ,record);
		}
		return true;
	case RALT_T(KC_DOT):
		// use default process other than KANA layer
		if(!is_default_layer_kana()) return true;
		// use default process for mod
		if(record->tap.count > 0) {
			return process_ime(KANA_DOT ,record);
		}
		return true;
	case LSFT_T(KC_SPC):
		// use default process other than KANA layer
		if(!is_default_layer_kana()) return true;
		// use default process for mod
		if(record->tap.count > 0) {
			return process_ime(IM_LSPC ,record);
		}
		return true;
	case RSFT_T(KC_SPC):
		// use default process other than KANA layer
		if(!is_default_layer_kana()) return true;
		// use default process for mod
		if(record->tap.count > 0) {
			return process_ime(IM_RSPC ,record);
		}
		return true;
	case FN_T(KC_ENT):
		// use default process other than KANA layer
		if(!is_default_layer_kana()) return true;
		// use default process for mod
		if(record->tap.count > 0) {
			return process_ime(IM_LENT ,record);
		}
		return true;
	// use KC_PENT to distinguish between left and right
	case FN_T(KC_PENT):
		// use default process for mod
		if(record->tap.count > 0) {
			if(!is_default_layer_kana()) {
				if(event.pressed) {
					register_code(KC_ENT);
				}
				else { unregister_code(KC_ENT); }
				return false;
			}
			else { return process_ime(IM_RENT ,record); }
		}
		return true;
	case RCTL_T(KC_TAB):
		// use default process other than KANA layer
		if(!is_default_layer_kana()) return true;
		// use default process for mod
		if(record->tap.count > 0) {
			return process_ime(KC_TAB ,record);
		}
		return true;
	case LCTL_T(KC_ESC):
		// use default process other than KANA layer
		if(!is_default_layer_kana()) return true;
		// use default process for mod
		if(record->tap.count > 0) {
			return process_ime(KC_ESC ,record);
		}
		return true;
#endif
// To use japanese input specific keys on Windows,
// OS's layout should be jis layout.
// But I prefer us layout for symbol keys.
// So translate symbols from us layout to jis layout.
	case RGUI_T(JP_QUOT):
		// use default process for mod
		if(record->tap.count > 0) {
			if(event.pressed) {
				add_mods(MOD_LSFT);
				register_code(KC_7);
			}
			else {
				unregister_code(KC_7);
				del_mods(MOD_LSFT);
			}
			return false;
		}
		return true;
	default:
		break;
	}
	return true;
}

void led_set_user(uint8_t usb_led) {
	ergodox_led_all_set(LED_BRIGHTNESS_LO);

#ifndef NO_JAPANESE
	detect_ime_change(usb_led);
#endif

	if(IS_LED_ON(usb_led, USB_LED_CAPS_LOCK)) ergodox_right_led_2_on();
	else ergodox_right_led_2_off();

	// if(IS_LED_ON(usb_led, USB_LED_SCROLL_LOCK)) ergodox_right_led_3_on();
	// else ergodox_right_led_3_off();
}

#ifndef NO_JAPANESE
layer_state_t default_layer_state_set_user(layer_state_t state) {
	ergodox_led_all_set(LED_BRIGHTNESS_LO);
	uint8_t layer = biton32(state);
#ifdef ENABLE_STABLE_LAYER
// 	if(layer == L_STABLE) ergodox_right_led_1_on();
#endif
	if(layer == L_KANA) {
		if(is_practice_mode()) {
			ergodox_right_led_1_off();
			ergodox_right_led_3_on();
		}
		else {
			uint8_t im_state = get_im_state();
			if(im_state == IM_STATE_HIRAGANA_DIRECT) {
				ergodox_right_led_1_on();
				ergodox_right_led_3_on();
			}
			else if(im_state == IM_STATE_KATAKANA_DIRECT) {
				ergodox_right_led_1_on();
				ergodox_right_led_2_on();
			}
			else {
				ergodox_right_led_1_on();
				ergodox_right_led_3_off();
			}
		}
	}
	else {
		ergodox_right_led_1_off();
		ergodox_right_led_3_off();
	}
	return state;
}
#endif

void suspend_power_down_user(void) {
	ergodox_led_all_off();
}

void suspend_wakeup_init_user(void) {
	default_layer_state_set_user(default_layer_state);
}

uint16_t additional_kana_chord(uint32_t kana_chord) {
	if(kana_chord == BIT_CONSONANT_G) {
		return KANA_GA;
	}
	else if(kana_chord == BIT_CONSONANT_Z) {
		return KANA_ZI;
	}
	else if(kana_chord == BIT_CONSONANT_D) {
		return KANA_DA;
	}
	else if(kana_chord == BIT_CONSONANT_B) {
		return KANA_BA;
	}
	else if(kana_chord == BIT_CONSONANT_DH) {
		return KANA_DE;
	}
	else if(kana_chord == BIT_CONSONANT_X) {
		return KANA_DO;
	}
	return KC_NO;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[L_BASE] = LAYOUT_ergodox(
		// left hand
		KC_WH_L, KC_WH_R, LGUI_T(KC_SCLN), LALT_T(KC_COMM), LCTL_T(KC_ESC), KC_Z, KC_INS,
		KC_WH_U, KC_WH_D, KC_K, KC_I, KC_N, KC_F, KC_DEL,
		KC_PGUP, KC_PGDN, KC_Y, KC_E, KC_S, KC_M,
		KC_2, KC_1, KC_J, KC_U, KC_H, KC_P, KC_BSPC,
		KC_5, KC_4, KC_3, JP_LPRN, JP_RPRN,
		// thumb
		KC_RCLK, KC_LCLK,
		KC_MCLK,
		LSFT_T(KC_SPC), FN_T(KC_ENT), S(KC_CAPS),
		// right hand
		CLEAR, KC_Q, RCTL_T(KC_TAB), RALT_T(KC_DOT), RGUI_T(JP_QUOT), KC_HOME, KC_END,
		KC_SLSH, KC_W, KC_R, KC_O, KC_V, KC_LEFT, KC_RGHT,
		KC_C, KC_T, KC_A, KC_B, KC_DOWN, KC_UP,
		JP_DQT, KC_G, KC_L, KC_D, KC_X, KC_9, KC_0,
		JP_MINS, JP_UNDS, KC_6, KC_7, KC_8,
		// thumb
		KC_WBAK, KC_WFWD,
		KC_WSCH,
		IME, FN_T(KC_PENT), RSFT_T(KC_SPC)
	),
#ifdef ENABLE_STABLE_LAYER
	[L_STABLE] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, FN_T(KC_TRNS), LGUI_T(KC_TRNS), LALT_T(KC_TRNS), LCTL_T(KC_TRNS), KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		LSFT_T(KC_SPC), KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, RCTL_T(KC_TRNS), RALT_T(KC_TRNS), RGUI_T(KC_TRNS), FN_T(KC_TRNS), DF(L_BASE),
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, RSFT_T(KC_SPC)
	),
#endif
#ifndef NO_JAPANESE
	[L_KANA] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, IM_KATAKANA, VOWEL_I(KANA_I), VOWEL_E(KANA_RU), VOWEL_YE(KANA_MA), KC_TRNS,
		KC_TRNS, KC_TRNS, KANA_TU, VOWEL_O(KANA_NO), VOWEL_A(KANA_NN), VOWEL_YA(KANA_KU),
		KC_TRNS, KC_TRNS, IM_HIRAGANA, VOWEL_YO(KANA_KO), VOWEL_U(KANA_U), VOWEL_YU(KANA_XTU), KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, DF(L_BASE),
		KC_TRNS, CONSONANT_P(KANA_SU), CONSONANT_T(KANA_TA), CONSONANT_S(KANA_SI), KC_HENK, KC_TRNS, KC_TRNS,
		CONSONANT_M(KANA_NI), CONSONANT_R(KANA_TE), CONSONANT_K(KANA_KA), KANA_MO, KC_TRNS, KC_TRNS,
		KC_TRNS, CONSONANT_N(KANA_NA), CONSONANT_A(KANA_TO), CONSONANT_H(KANA_HA), KANA_ZSLSH, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
#endif
	[L_FN] = LAYOUT_ergodox(
		// left hand
		KC_NO, KC_NO, KC_LGUI, KC_LALT, KC_LCTL, KC_NO, KC_NO,
		KC_BRIU, KC_BRID, KC_SLCK, JP_COLN, JP_QUES, JP_AT, KC_TRNS,
		KC_F12, KC_F11, KC_APP, JP_LBRC, JP_RBRC, JP_HASH,
		KC_F2, KC_F1, KC_NO, JP_LCBR, JP_RCBR, JP_DLR, KC_TRNS,
		KC_F5, KC_F4, KC_F3, JP_GRV, JP_BSLS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_LSFT, KC_TRNS, KC_TRNS,
		// right hand
		CLEAR, KC_NO, KC_RCTL, KC_RALT, KC_RGUI, KC_MPRV, KC_MNXT,
		KC_TRNS, JP_PIPE, JP_EXLM, JP_ASTR, KC_PAUS, KC_MUTE, KC_MPLY,
		JP_AMPR, JP_EQL, JP_PLUS, KC_PSCR, KC_VOLD, KC_VOLU,
		KC_TRNS, JP_CIRC, JP_LT, JP_GT, KC_NO, KC_F9, KC_F10,
		JP_PERC, JP_TILD, KC_F6, KC_F7, KC_F8,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		PRACTICE_MODE, KC_TRNS, KC_RSFT
	),
};

