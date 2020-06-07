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

static bool coln_pressed = false;
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
	matrix_scan_kana();
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
		break;
	case IME:
		if(event.pressed) {
			register_code(JP_ZHTG);
		}
		else {
#ifndef NO_JAPANESE
			uint8_t default_layer = biton32(default_layer_state);
			// TODO: use raw_hid to detect ime state
			if(default_layer == L_BASE) {
				numlock_on();
			}
			else if(default_layer == L_KANA) {
				numlock_off();
			}
			is_practice_mode = false;
#endif
			unregister_code(JP_ZHTG);
		}
		return false;
		break;
#ifndef NO_JAPANESE
	case COMMIT_MODE:
		if(event.pressed) {
			is_commit_mode = !is_commit_mode;
			is_practice_mode = false;
			// to update led indicator
			default_layer_state_set_user(default_layer_state);
		}
		return false;
		break;
	case PRACTICE_MODE:
#ifndef NO_JAPANESE
		if(event.pressed) {
			uint8_t default_layer = biton32(default_layer_state);
			if(default_layer == L_BASE) {
				numlock_on();
				is_practice_mode = true;
			}
			//L_KANA
			else {
				tap_code(JP_ZHTG);
				is_practice_mode = !is_practice_mode;
			}
			is_commit_mode = false;
			// to update led indicator
			default_layer_state_set_user(default_layer_state);
		}
#endif
		return false;
#endif
// To use japanese input specific keys on Windows,
// OS's layout should be jis layout.
// But I prefer us layout for symbol keys.
// So translate symbols from us layout to jis layout.
// The symbol shifted on us layout but on jis should be unshifted.
	case RGUI_T(JP_QUOT):
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
	/*
	case JIS_COLN:
		if(event.pressed) {
			del_mods(MOD_LSFT);
			register_code(JP_COLN);
			coln_pressed = true;
		}
		else {
			if(layer_state & (1UL<<L_SHIFT)) {
				unregister_code(JP_COLN);
				add_mods(MOD_LSFT);
				coln_pressed = false;
			}
			else {
				unregister_code(KC_SCLN);
			}
		}
		return false;
	case LCTL_T(JP_AT):
		if(record->tap.count > 0) {
			if(event.pressed) {
				del_mods(MOD_LSFT);
				register_code(JP_AT);
			}
			else {
				unregister_code(JP_AT);
				add_mods(MOD_LSFT);
			}
			return false;
		}
		return true;
	case RGUI_T(JP_CIRC):
		if(record->tap.count > 0) {
			if(event.pressed) {
				del_mods(MOD_LSFT);
				register_code(JP_CIRC);
			}
			else {
				unregister_code(JP_CIRC);
				add_mods(MOD_LSFT);
			}
			return false;
		}
		return true;
	*/
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

static uint8_t prev_layer_state = 1UL<<L_BASE;
layer_state_t layer_state_set_user(layer_state_t state) {
	if(state & (1UL<<L_SHIFT) && !(prev_layer_state & (1UL<<L_SHIFT))) {
		register_code(KC_LSFT);
	}
	else if(!(state & (1UL<<L_SHIFT)) && prev_layer_state & (1UL<<L_SHIFT)) {
		if(coln_pressed) {
			coln_pressed = false;
			unregister_code(JP_COLN);
			unregister_code(KC_LSFT);
			register_code(KC_SCLN);
		}
		else unregister_code(KC_LSFT);
	}
	prev_layer_state = state;
	return state;
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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[L_BASE] = LAYOUT_ergodox(
		// left hand
		KC_WH_L, KC_WH_R, LGUI_T(KC_SCLN), LALT_T(KC_COMM), LCTL_T(KC_ESC), KC_Z, KC_INS,
		KC_WH_U, KC_WH_D, KC_K, KC_I, KC_N, KC_F, KC_DEL,
		KC_2, KC_1, KC_Y, KC_E, KC_S, KC_M,
		KC_4, KC_3, KC_J, KC_U, KC_H, KC_P, KC_BSPC,
		KC_6, KC_5, KC_COMM, JP_LPRN, JP_RPRN,
		// thumb
		KC_RCLK, KC_LCLK,
		KC_MCLK,
		LSFT_T(KC_SPC), FN_T(KC_ENT), S(KC_CAPS),
		// right hand
		CLEAR, KC_Q, RCTL_T(KC_TAB), RALT_T(KC_DOT), RGUI_T(JP_QUOT), KC_HOME, KC_END,
		KC_SLSH, KC_W, KC_R, KC_O, KC_V, KC_LEFT, KC_RGHT,
		KC_C, KC_T, KC_A, KC_G, KC_DOWN, KC_UP,
		JP_DQT, KC_B, KC_L, KC_D, KC_X, KC_0, KC_7,
		JP_MINS, JP_UNDS, KC_DOT, KC_8, KC_9,
		// thumb
		KC_PGDN, KC_PGUP,
		KC_WBAK,
		IME, FN_T(KC_ENT), RSFT_T(KC_SPC)
	),
	/*
	// translate jis to us
	[L_SHIFT] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, LCTL_T(JP_AT), KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, JP_PLUS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, JP_ASTR, RCTL_T(JP_LPRN), RALT_T(JP_RPRN), RGUI_T(JP_CIRC), FN_T(JP_AMPR), DF(L_BASE),
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, JP_UNDS, JP_DQT, JP_TILD,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, JIS_COLN,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	*/
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
		KC_TRNS, KANA_ZDOT, KANA_DOT, KANA_K_T(KANA_KA), KANA_T_T(KANA_TA), KANA_TH_T(KANA_SU), KC_TRNS,
		KC_TRNS, KANA_Q_T(KANA_KI), KANA_F_T(KANA_KO), KANA_S_T(KANA_SI), KANA_R_T(KANA_TO), KANA_N_T(KANA_NA),
		KC_TRNS, KANA_ZMINS, KC_MINS, KANA_H_T(KANA_HA), KANA_X_T(KANA_TE), KANA_W_T(KANA_MO), KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, DF(L_BASE),
		KC_TRNS, KANA_YE_T(KANA_MA), KANA_E_T(KANA_RU), KANA_I_T(KANA_I), KANA_GA, KANA_ZSLSH, KC_TRNS,
		KANA_YA_T(KANA_KU), KANA_A_T(KANA_NN), KANA_O_T(KANA_NO), KANA_NI, KANA_DE, KC_TRNS,
		KC_TRNS, KANA_YU_T(KANA_XTU), KANA_U_T(KANA_U), KANA_YO_T(KANA_COMM), KANA_WO, KANA_ZL, KC_TRNS,
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
		KC_F2, KC_F1, KC_F11, JP_LBRC, JP_RBRC, JP_HASH,
		KC_F4, KC_F3, KC_F12, JP_LCBR, JP_RCBR, JP_DLR, KC_TRNS,
		KC_F6, KC_F5, KC_NO, JP_GRV, JP_BSLS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_LSFT, KC_TRNS, KC_TRNS,
		// right hand
		CLEAR, KC_NO, KC_RCTL, KC_RALT, KC_RGUI, KC_MPRV, KC_MNXT,
		KC_TRNS, JP_PIPE, JP_EXLM, JP_ASTR, KC_APP, KC_MUTE, KC_MPLY,
		JP_AMPR, JP_EQL, JP_PLUS, KC_PSCR, KC_VOLD, KC_VOLU,
		KC_TRNS, JP_CIRC, JP_LT, JP_GT, KC_PAUS, KC_F10, KC_F7,
		JP_PERC, JP_TILD, KC_NO, KC_F8, KC_F9,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		PRACTICE_MODE, KC_TRNS, KC_RSFT
	),
#ifndef NO_JAPANESE
	[L_KANA_A] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_NO, KC_NO, KANA_GA, KANA_DA, KC_NO, KC_TRNS,
		KC_TRNS, KANA_GWA, KANA_VA, KANA_ZA, KANA_MA, KANA_PA,
		KC_TRNS, KC_NO, KC_NO, KANA_BA, KANA_XA, KANA_WHA, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_F7, COMMIT_MODE, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, DF(L_BASE),
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KANA_ROLL, KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
        KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_KANA_I] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_NO, KC_NO, KANA_GI, KANA_DI, KANA_DHI, KC_TRNS,
		KC_TRNS, KANA_GWI, KANA_VI, KANA_ZI, KANA_MI, KANA_PI,
		KC_TRNS, KC_NO, KC_NO, KANA_BI, KANA_XI, KANA_WI, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_MHEN, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, DF(L_BASE),
		KC_TRNS, KANA_ROLL, KANA_ROLL, KC_TRNS, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
        KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_KANA_U] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_NO, KC_NO, KANA_GU, KANA_DU, KANA_DWU, KC_TRNS,
		KC_TRNS, KANA_GU, KANA_VU, KANA_ZU, KANA_MU, KANA_PU,
		KC_TRNS, KC_NO, KC_NO, KANA_BU, KANA_XU, KANA_U, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, DF(L_BASE),
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
        KC_TRNS, KANA_ROLL, KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_KANA_E] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_NO, KC_NO, KANA_GE, KANA_DE, KC_NO, KC_TRNS,
		KC_TRNS, KANA_GWE, KANA_VE, KANA_ZE, KANA_ME, KANA_PE,
		KC_TRNS, KC_NO, KC_NO, KANA_BE, KANA_XE, KANA_WE, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_KANA, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, DF(L_BASE),
		KC_TRNS, KANA_ROLL, KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
        KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_KANA_O] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_NO, KC_NO, KANA_GO, KANA_DO, KC_NO, KC_TRNS,
		KC_TRNS, KANA_GWO, KANA_VO, KANA_ZO, KANA_MO, KANA_PO,
		KC_TRNS, KC_NO, KC_NO, KANA_BO, KANA_XO, KANA_WHO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_F6, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, DF(L_BASE),
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KANA_ROLL, KANA_ROLL, KC_TRNS, KANA_ROLL, KANA_ROLL, KC_TRNS,
        KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_KANA_YA] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_NO, KC_NO, KANA_GYA, KANA_DYA, KANA_DHA, KC_TRNS,
		KC_TRNS, KANA_XKA, KANA_VYA, KANA_JA, KANA_MYA, KANA_PYA,
		KC_TRNS, KC_NO, KC_NO, KANA_BYA, KANA_XYA, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, DF(L_BASE),
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
        KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_KANA_YU] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_NO, KC_NO, KANA_GYU, KANA_DYU, KANA_DHU, KC_TRNS,
		KC_TRNS, KC_NO, KANA_VYU, KANA_JU, KANA_MYU, KANA_PYU,
		KC_TRNS, KC_NO, KC_NO, KANA_BYU, KANA_XYU, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, DF(L_BASE),
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
        KC_TRNS, KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_KANA_YO] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_NO, KC_NO, KANA_GYO, KANA_DYO, KANA_DHO, KC_TRNS,
		KC_TRNS, KC_NO, KANA_VYO, KANA_JO, KANA_MYO, KANA_PYO,
		KC_TRNS, KC_NO, KC_NO, KANA_BYO, KANA_XYO, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, DF(L_BASE),
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
        KC_TRNS, KANA_ROLL, KANA_ROLL, KC_TRNS, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_KANA_YE] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_NO, KC_NO, KANA_GYE, KANA_DYE, KC_NO, KC_TRNS,
		KC_TRNS, KANA_XKE, KC_NO, KANA_JE, KANA_MYE, KANA_PYE,
		KC_TRNS, KC_NO, KC_NO, KANA_BYE, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, DF(L_BASE),
		KC_TRNS, KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
        KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_KANA_X] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, CLEAR,
		KC_TRNS, KANA_YE, KANA_E, KANA_I, KC_NO, KC_NO, KC_TRNS,
		KANA_YA, KANA_A, KANA_O, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KANA_YU, KANA_U, KANA_YO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_KANA_K] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KC_TRNS, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, CLEAR,
		KC_TRNS, KANA_KYE, KANA_KE, KANA_KI, KC_NO, KC_NO, KC_TRNS,
		KANA_KYA, KANA_KA, KANA_KO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KANA_KYU, KANA_KU, KANA_KYO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_HENK
	),
	[L_KANA_S] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KC_TRNS, KANA_ROLL, KANA_ROLL,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, CLEAR,
		KC_TRNS, KANA_SYE, KANA_SE, KANA_SI, KC_NO, KC_NO, KC_TRNS,
		KANA_SYA, KANA_SA, KANA_SO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KANA_SYU, KANA_SU, KANA_SYO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_F9
	),
	[L_KANA_T] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS, KANA_ROLL, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, CLEAR,
		KC_TRNS, KANA_TYE, KANA_TE, KANA_TI, KC_NO, KC_NO, KC_TRNS,
		KANA_TYA, KANA_TA, KANA_TO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KANA_TYU, KANA_TU, KANA_TYO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_CAPS
	),
	[L_KANA_N] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_NO, KANA_ROLL, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, CLEAR,
		KC_TRNS, KANA_NYE, KANA_NE, KANA_NI, KC_NO, KC_NO, KC_TRNS,
		KANA_NYA, KANA_NA, KANA_NO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KANA_NYU, KANA_NU, KANA_NYO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_KANA_H] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KC_TRNS, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, CLEAR,
		KC_TRNS, KANA_HYE, KANA_HE, KANA_HI, KC_NO, KC_NO, KC_TRNS,
		KANA_HYA, KANA_HA, KANA_HO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KANA_HYU, KANA_HU, KANA_HYO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	// [L_KANA_M] = LAYOUT_ergodox(
	// 	// left hand
	// 	KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
	// 	KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
	// 	KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL,
	// 	KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS, KC_TRNS,
	// 	KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
	// 	// thumb
	// 	KC_TRNS, KC_TRNS,
	// 	KC_TRNS,
	// 	KC_TRNS, KC_TRNS, KC_TRNS,
	// 	// right hand
	// 	KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, CLEAR,
	// 	KC_TRNS, KC_NO, KANA_ME, KANA_MI, KC_NO, KC_NO, KC_TRNS,
	// 	KANA_MYA, KANA_MA, KANA_MO, KC_NO, KC_NO, KC_TRNS,
	// 	KC_TRNS, KANA_MYU, KANA_MU, KANA_MYO, KC_NO, KC_NO, KC_TRNS,
	// 	KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
	// 	// thumb
	// 	KC_TRNS, KC_TRNS,
	// 	KC_TRNS,
	// 	KC_TRNS, KC_TRNS, KC_TRNS
	// ),
	[L_KANA_R] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_NO, KANA_ROLL,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, CLEAR,
		KC_TRNS, KANA_RYE, KANA_RE, KANA_RI, KC_NO, KC_NO, KC_TRNS,
		KANA_RYA, KANA_RA, KANA_RO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KANA_RYU, KANA_RU, KANA_RYO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		JP_ZHTG, KC_TRNS, KC_F10
	),
	[L_KANA_W] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, CLEAR,
		KC_TRNS, KC_NO, KANA_WYE, KANA_WYI, KC_NO, KC_NO, KC_TRNS,
		KANA_XWA, KANA_WA, KANA_WO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_NO, KANA_U, KC_NO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_KANA_F] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KANA_ROLL, KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, CLEAR,
		KC_TRNS, KC_NO, KANA_FE, KANA_FI, KC_NO, KC_NO, KC_TRNS,
		KANA_FYA, KANA_FA, KANA_FO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KANA_FYU, KANA_HU, KANA_FYO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_KANA_Q] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, CLEAR,
		KC_TRNS, KC_NO, KANA_QE, KANA_QI, KC_NO, KC_NO, KC_TRNS,
		KANA_QWA, KANA_QA, KANA_QO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_NO, KANA_KU, KC_NO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_KANA_TH] = LAYOUT_ergodox(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS, KC_TRNS,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL,
		KC_TRNS, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KANA_ROLL, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, CLEAR,
		KC_TRNS, KC_NO, KANA_TSE, KANA_THI, KC_NO, KC_NO, KC_TRNS,
		KANA_THA, KANA_TSA, KANA_TSO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KANA_THU, KANA_TWU, KANA_THO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	#endif
};

