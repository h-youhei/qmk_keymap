#include "ergodox_ez.h"
#include "util_user.h"
#include "action.h"
#include "keymap_jp.h"

#include "my.h"

#ifndef NO_JAPANESE
#include "kana.h"
#include "kana_chord.h"
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

void numlock_on(void) {
	if(IS_HOST_LED_OFF(USB_LED_NUM_LOCK)) {
		tap_code(KC_NLCK);
	}
}
void numlock_off(void) {
	if(IS_HOST_LED_ON(USB_LED_NUM_LOCK)) {
		tap_code(KC_NLCK);
	}
}

void matrix_scan_user(void) {
#ifndef NO_JAPANESE
	// matrix_scan_kana();
#endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	keyevent_t event = record->event;
#ifndef NO_JAPANESE
	if(!process_kana(keycode, record)) return false;
#endif
	switch(keycode) {
	case CLEAR:
		if(event.pressed) {
			clear_mods();
			layer_clear();
		}
		return false;
	case IME:
		if(event.pressed) {
			tap_code(JP_ZHTG);
#ifndef NO_JAPANESE
			uint8_t default_layer = biton32(default_layer_state);
			// TODO: use raw_hid to detect ime state
			if(default_layer == L_BASE) {
				numlock_on();
			}
			// L_KANA
			else {
				if(!is_practice_mode) {
					numlock_off();
				}
				else {
					is_practice_mode = false;
					default_layer_state_set_user(default_layer_state);
				}
			}
#endif
		}
		return false;
#ifndef NO_JAPANESE
	case COMMIT_MODE:
		if(event.pressed) {
			is_commit_mode = !is_commit_mode;
			is_practice_mode = false;
			// to update led indicator
			default_layer_state_set_user(default_layer_state);
		}
		return false;
	case PRACTICE_MODE:
		if(event.pressed) {
			uint8_t default_layer = biton32(default_layer_state);
			is_commit_mode = false;
			if(default_layer == L_BASE) {
				is_practice_mode = true;
				numlock_on();
			}
			//L_KANA
			else {
				if(is_practice_mode) {
					is_practice_mode = false;
					numlock_off();
				}
				// IME is turned on
				else {
					tap_code(JP_ZHTG);
					is_practice_mode = true;
					// to update led indicator
					default_layer_state_set_user(default_layer_state);
				}
			}
		}
		return false;
	case LALT_T(KC_COMM):
		// use default process other than KANA layer
		if(biton32(default_layer_state | layer_state) != L_KANA) {
			return true;
		}
		// use default process for mod
		if(record->tap.count > 0) {
			return process_record_kana(KANA_COMM ,record);
		}
		return true;
	case RALT_T(KC_DOT):
		// use default process other than KANA layer
		if(biton32(default_layer_state | layer_state) != L_KANA) {
			return true;
		}
		// use default process for mod
		if(record->tap.count > 0) {
			return process_record_kana(KANA_DOT ,record);
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
	// use Num_Lock to recognize that
	// modal editor change ime state
	if(IS_LED_ON(usb_led, USB_LED_NUM_LOCK)) {
		default_layer_set(1UL << L_KANA);
	}
	else default_layer_set(1UL << L_BASE);
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
		if(is_commit_mode) {
			ergodox_right_led_1_on();
			ergodox_right_led_3_on();
		}
		else if(is_practice_mode) {
			ergodox_right_led_1_off();
			ergodox_right_led_3_on();
		}
		else {
			ergodox_right_led_1_on();
			ergodox_right_led_3_off();
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

bool additioral_kana_chord(uint32_t kana_chord) {
	if(kana_chord == BIT_CONSONANT_G) {
		register_kana(KANA_GA);
	}
	else if(kana_chord == BIT_CONSONANT_Z) {
		register_kana(KANA_ZI);
	}
	else if(kana_chord == BIT_CONSONANT_D) {
		register_kana(KANA_DA);
	}
	else if(kana_chord == BIT_CONSONANT_B) {
		register_kana(KANA_BA);
	}
	else if(kana_chord == BIT_CONSONANT_DH) {
		register_kana(KANA_DE);
	}
	else if(kana_chord == BIT_CONSONANT_X) {
		register_kana(KANA_DO);
	}
	else return true;
	return false;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[L_BASE] = LAYOUT_ergodox(
		// left hand
		KC_WH_L, KC_WH_R, LGUI_T(KC_SCLN), LALT_T(KC_COMM), LCTL_T(KC_ESC), KC_Z, KC_INS,
		KC_WH_U, KC_WH_D, KC_K, KC_I, KC_N, KC_F, KC_DEL,
		KC_1, KC_0, KC_Y, KC_E, KC_S, KC_M,
		KC_3, KC_2, KC_J, KC_U, KC_H, KC_P, KC_BSPC,
		KC_6, KC_5, KC_4, JP_LPRN, JP_RPRN,
		// thumb
		KC_RCLK, KC_LCLK,
		KC_MCLK,
		LSFT_T(KC_SPC), FN_T(KC_ENT), S(KC_CAPS),
		// right hand
		CLEAR, KC_Q, RCTL_T(KC_TAB), RALT_T(KC_DOT), RGUI_T(JP_QUOT), KC_HOME, KC_END,
		KC_SLSH, KC_W, KC_R, KC_O, KC_V, KC_LEFT, KC_RGHT,
		KC_C, KC_T, KC_A, KC_B, KC_DOWN, KC_UP,
		JP_DQT, KC_G, KC_L, KC_D, KC_X, KC_PGDN, KC_PGUP,
		JP_MINS, JP_UNDS, KC_7, KC_8, KC_9,
		// thumb
		KC_WBAK, KC_WFWD,
		KC_WSCH,
		IME, FN_T(KC_ENT), RSFT_T(KC_SPC)
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
		KC_TRNS, KC_TRNS, KC_MHEN, VOWEL_I(KANA_I), VOWEL_E(KANA_RU), VOWEL_YE(KANA_MA), KC_TRNS,
		KC_TRNS, KC_TRNS, KANA_TU, VOWEL_O(KANA_NO), VOWEL_A(KANA_NN), VOWEL_YA(KANA_KU),
		KC_TRNS, KC_TRNS, KC_KANA, VOWEL_YO(KANA_KO), VOWEL_U(KANA_U), VOWEL_YU(KANA_XTU), KC_TRNS,
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
		KC_F4, KC_F5, KC_LGUI, KC_LALT, KC_LCTL, KC_NO, KC_NO,
		KC_F2, KC_F3, KC_SLCK, JP_COLN, JP_QUES, JP_AT, KC_TRNS,
		KC_F10, KC_F1, KC_F11, JP_LBRC, JP_RBRC, JP_HASH,
		KC_BRIU, KC_BRID, KC_F12, JP_LCBR, JP_RCBR, JP_DLR, KC_TRNS,
		KC_NO, KC_NO, KC_NO, JP_GRV, JP_BSLS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_LSFT, KC_TRNS, KC_TRNS,
		// right hand
		CLEAR, KC_NO, KC_RCTL, KC_RALT, KC_RGUI, KC_MPRV, KC_MNXT,
		KC_TRNS, JP_PIPE, JP_EXLM, JP_ASTR, KC_APP, KC_MUTE, KC_MPLY,
		JP_AMPR, JP_EQL, JP_PLUS, KC_PSCR, KC_VOLD, KC_VOLU,
		KC_TRNS, JP_CIRC, JP_LT, JP_GT, KC_PAUS, KC_F6, KC_F7,
		JP_PERC, JP_TILD, KC_NO, KC_F8, KC_F9,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		PRACTICE_MODE, KC_TRNS, KC_RSFT
	),
};

