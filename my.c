#include "ergodox_ez.h"
#include "util_user.h"
#include "action.h"

// To use japanese input specific keys on Windows,
// OS's layout should be jis layout
// even I prefer us layout for symbol keys.
#include "keymap_jp.h"

#include "my.h"

#ifndef NO_JAPANESE
#include "raw_hid.h"
#include "kana.h"
#include "kana_chord.h"
#include "ime.h"
#endif

#define SFT_LT(kc) LT(L_SHIFT, kc)
#define NUM_SIGN_T(kc) LT(L_NUM_SIGN, kc)
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
	case RCTL_T(KC_TAB):
	case LCTL_T(KC_ESC):
	case LALT_T(KC_COMM):
	case RALT_T(KC_DOT):
	case LGUI_T(KC_SCLN):
	case LSFT_T(KC_SPC):
	case RSFT_T(KC_SPC):
	case NUM_SIGN_T(KC_ENT):
	case FN_T(HIRAGANA):
	case FN_T(KATAKANA):
	case LCTL_T(JP_AT):
	case RCTL_T(KC_7):
	case LALT_T(JP_COLN):
	case RALT_T(KC_8):
	case RGUI_T(KC_9):
	{
		uint16_t keycode_8 = keycode & 0xFF;
		// use default process other than KANA layer
		if(!is_default_layer_kana()) return true;
		// use default process for mod
		if(record->tap.count > 0) {
			return process_ime(keycode_8, record);
		}
		return true;
	}
#endif
// recover shift that truncated in MOD_T() macro
	case RGUI_T(JP_QUOT):
	case LGUI_T(JP_GRV):
		// use default process for mod
		if(record->tap.count > 0) {
			uint16_t keycode_8 = keycode & 0xFF;
			if(event.pressed) {
				if(!is_default_layer_kana()) {
					add_mods(MOD_LSFT);
					register_code(keycode_8);
				}
				else {
					return process_ime(LSFT(keycode_8), record);
				}
			}
			else {
				if(!is_default_layer_kana()) {
					unregister_code(keycode_8);
					del_mods(MOD_LSFT);
				}
				else {
					return process_ime(LSFT(keycode_8), record);
				}
			}
			return false;
		}
		return true;
	default:
		break;
	}
#ifndef NO_JAPANESE
	if(!process_ime(keycode, record)) return false;
#endif
	return true;
}

void led_set_user(uint8_t usb_led) {
	ergodox_led_all_set(LED_BRIGHTNESS_LO);

	if(biton32(default_layer_state) != L_KANA) {
		if(IS_LED_ON(usb_led, USB_LED_CAPS_LOCK)) ergodox_right_led_2_on();
		else ergodox_right_led_2_off();
	}

	// if(IS_LED_ON(usb_led, USB_LED_NUM_LOCK)) ergodox_right_led_1_on();
	// else ergodox_right_led_1_off();
	// if(IS_LED_ON(usb_led, USB_LED_SCROLL_LOCK)) ergodox_right_led_3_on();
	// else ergodox_right_led_3_off();
}

#ifdef ENABLE_STABLE_LAYER
layer_state_t default_layer_state_set_user(layer_state_t state) {
	ergodox_led_all_set(LED_BRIGHTNESS_LO);
	uint8_t layer = biton32(state);
// 	if(layer == L_STABLE) ergodox_right_led_1_on();
	return state;
}
#endif

#ifndef NO_JAPANESE
void raw_hid_receive(uint8_t *data, uint8_t length) {
	uint8_t *command_id = &(data[0]);
	switch(*command_id) {
		case id_im_off:
			on_im_off();
			break;
		case id_im_on:
			on_im_on();
			break;
	}
}
#endif

#ifndef NO_JAPANESE
void im_state_set_user(uint8_t state) {
	ergodox_led_all_set(LED_BRIGHTNESS_LO);
	uint8_t layer = biton32(default_layer_state);
	if(layer == L_KANA) {
		if(is_practice_mode()) {
			ergodox_right_led_1_off();
			ergodox_right_led_3_on();
		}
		else {
			switch(state) {
			case IM_STATE_HIRAGANA_DIRECT:
				ergodox_right_led_1_on();
				ergodox_right_led_3_on();
				break;
			case IM_STATE_KATAKANA_DIRECT:
				ergodox_right_led_1_on();
				ergodox_right_led_2_on();
				break;
			default:
				ergodox_right_led_1_on();
				ergodox_right_led_2_off();
				ergodox_right_led_3_off();
				break;
			}
		}
	}
	else {
		ergodox_right_led_1_off();
		ergodox_right_led_3_off();
	}
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
		KC_WH_L, KC_WH_R, LGUI_T(KC_SCLN), LALT_T(KC_COMM), LCTL_T(KC_ESC), FN_T(KC_Z), KC_INS,
		KC_WH_U, KC_WH_D, KC_K, KC_I, KC_N, KC_F, KC_DEL,
		KC_RCLK, KC_LCLK, KC_Y, KC_E, KC_S, KC_M,
		KC_F5, KC_MCLK, KC_J, KC_U, KC_H, KC_P, KC_BSPC,
		KC_WBAK, KC_WFWD, KC_NO, JP_LPRN, JP_RPRN,
		// thumb
		KC_NO, KC_NO,
		KC_NO,
		LSFT_T(KC_SPC), NUM_SIGN_T(KC_ENT), S(KC_CAPS),
		// right hand
		CLEAR, FN_T(KC_Q), RCTL_T(KC_TAB), RALT_T(KC_DOT), RGUI_T(JP_QUOT), KC_HOME, KC_END,
		KC_SLSH, KC_W, KC_R, KC_O, KC_V, KC_LEFT, KC_RGHT,
		KC_C, KC_T, KC_A, KC_B, KC_DOWN, KC_UP,
		JP_DQT, KC_G, KC_L, KC_D, KC_X, KC_PGDN, KC_PGUP,
		JP_MINS, JP_UNDS, KC_NO, KC_NO, CLEAR,
		// thumb
		KC_NO, KC_NO,
		KC_NO,
		JP_ZHTG, NUM_SIGN_T(KC_ENT), RSFT_T(KC_SPC)
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
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, FN_T(HIRAGANA), KC_TRNS,
		KC_TRNS, KC_TRNS, KANA_TU, VOWEL_I(KANA_I), VOWEL_E(KANA_RU), VOWEL_YE(KANA_MA), KC_TRNS,
		KC_TRNS, KC_TRNS, KANA_DE, VOWEL_O(KANA_NO), VOWEL_A(KANA_NN), VOWEL_YA(KANA_KU),
		KC_TRNS, KC_TRNS, KANA_ZSLSH, VOWEL_YO(KANA_KO), VOWEL_U(KANA_U), VOWEL_YU(KANA_XTU), KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_CAPS,
		// right hand
		KC_TRNS, FN_T(KATAKANA), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, CONSONANT_P(KANA_SU), CONSONANT_T(KANA_TA), CONSONANT_S(KANA_SI), KANA_MO, KC_TRNS, KC_TRNS,
		CONSONANT_M(KANA_NI), CONSONANT_R(KANA_TE), CONSONANT_K(KANA_KA), KANA_GA, KC_TRNS, KC_TRNS,
		KC_TRNS, CONSONANT_N(KANA_NA), CONSONANT_A(KANA_TO), CONSONANT_H(KANA_HA), KANA_ZDOT, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
#endif
	[L_NUM_SIGN] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, LGUI_T(JP_GRV), LALT_T(JP_COLN), LCTL_T(JP_AT), KC_TRNS, KC_NO,
		KC_TRNS, KC_TRNS, JP_BSLS, JP_QUES, JP_PLUS, JP_ASTR, KC_TRNS,
		KC_TRNS, KC_TRNS, JP_TILD, JP_EXLM, JP_EQL, JP_AMPR,
		KC_TRNS, KC_TRNS, JP_CIRC, JP_LT, JP_GT, JP_PIPE, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_NO, JP_LCBR, JP_RCBR,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, RCTL_T(KC_7), RALT_T(KC_8), RGUI_T(KC_9), KC_TRNS, KC_TRNS,
		KC_TRNS, JP_DLR, KC_4, KC_5, KC_6, KC_TRNS, KC_TRNS,
		JP_HASH, KC_1, KC_2, KC_3, KC_TRNS, KC_TRNS,
		KC_TRNS, JP_PERC, KC_0, KC_COMM, KC_DOT, KC_TRNS, KC_TRNS,
		JP_LBRC, JP_RBRC, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		PRACTICE_MODE, KC_TRNS, KC_TRNS
	),
	[L_FN] = LAYOUT_ergodox(
		// left hand
		KC_NO, KC_NO, KC_LGUI, KC_LALT, KC_LCTL, KC_TRNS, KC_NO,
		KC_NO, KC_SLCK, KC_KANA, KC_MHEN, KC_HENK, KC_NO, KC_TRNS,
		KC_APP, KC_NO, KC_NO, KC_NO, KC_PSCR, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_PAUS, KC_TRNS,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_LSFT, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, RCTL_T(KC_F7), RALT_T(KC_F8), RGUI_T(KC_F9), KC_MPRV, KC_MNXT,
		KC_TRNS, KC_NO, KC_F4, KC_F5, KC_F6, KC_MUTE, KC_MPLY,
		KC_NO, KC_F1, KC_F2, KC_F3, KC_VOLD, KC_VOLU,
		KC_TRNS, KC_NO, KC_F10, KC_F11, KC_F12, KC_BRID, KC_BRIU,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		PRACTICE_MODE, KC_TRNS, KC_TRNS
	),
};

