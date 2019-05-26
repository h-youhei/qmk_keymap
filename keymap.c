#include "ergodox_ez.h"
#include "util_user.h"
#include "action.h"

enum user_code {
	CLEAR = SAFE_RANGE,
	DF_NUM,
};

enum Layer {
	L_BASE,
#ifdef ENABLE_STABLE_LAYER
	L_STABLE,
#endif
	L_FN,
};
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


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch(keycode) {
	case CLEAR:
		if(record->event.pressed) {
			clear_mods();
			layer_clear();
		}
		return false;
	default:
		break;
	}

#ifdef ENABLE_STABLE_LAYER
	uint8_t layer = biton32(default_layer_state);
	if(layer == L_STABLE) return true;
#endif
	return true;
}

void led_set_user(uint8_t usb_led) {
	ergodox_led_all_set(LED_BRIGHTNESS_LO);

	if(IS_LED_ON(usb_led, USB_LED_CAPS_LOCK)) ergodox_right_led_2_on();
	else ergodox_right_led_2_off();

	if(IS_LED_ON(usb_led, USB_LED_SCROLL_LOCK)) ergodox_right_led_3_on();
	else ergodox_right_led_3_off();
}

#ifdef ENABLE_STABLE_LAYER
uint32_t default_layer_state_set_user(uint32_t state) {
	uint8_t layer = biton32(state);
	if(layer == L_STABLE) ergodox_right_led_1_on();
	else ergodox_right_led_1_off();
	return state;
}
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[L_BASE] = LAYOUT_ergodox(
		// left hand
		KC_PSCR, FN_T(KC_4), LGUI_T(KC_5), LALT_T(KC_1), LCTL_T(KC_2), KC_3, KC_INS,
		KC_BSLS, KC_Q, KC_B, KC_R, KC_N, KC_K, KC_EQL,
		KC_SLSH, KC_F, KC_C, KC_T, KC_S, KC_L,
		KC_CAPS, KC_X, KC_W, KC_M, KC_H, KC_V, KC_BSPC,
		KC_NO, KC_NO, KC_HOME, KC_UP, KC_DOWN,
		// thumb
		KC_RCLK, KC_LCLK,
		KC_MCLK,
		LSFT_T(KC_SPC), KC_ENT, KC_ESC,
		// right hand
		KC_LBRC, KC_8, RCTL_T(KC_9), RALT_T(KC_0), RGUI_T(KC_6), FN_T(KC_7), CLEAR,
		KC_RBRC, KC_Y, KC_I, KC_O, KC_G, KC_Z, KC_GRV,
		KC_U, KC_E, KC_A, KC_D, KC_P, KC_SCLN,
		KC_DEL, KC_MINS, KC_COMM, KC_DOT, KC_QUOT, KC_J, KC_CAPS,
		KC_LEFT, KC_RGHT, KC_END, KC_NO, KC_NO,
		// thumb
		KC_PGDN, KC_PGUP,
		KC_WBAK,
		KC_KANA, KC_TAB, RSFT_T(KC_SPC)
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
	[L_FN] = LAYOUT_ergodox(
		// left hand
		KC_NO, FN_T(KC_F4), LGUI_T(KC_F5), LALT_T(KC_F1), LCTL_T(KC_F2), KC_F3, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_F11, KC_F12, KC_NO, KC_TRNS,
		KC_NO, KC_NO, KC_MUTE, KC_VOLD, KC_VOLU, KC_MPLY,
		KC_NO, KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL, KC_NO, KC_TRNS,
		KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_NO, KC_F8, RCTL_T(KC_F9), RALT_T(KC_F10), RGUI_T(KC_F6), FN_T(KC_F7), CLEAR,
		KC_TRNS, KC_NO, KC_BRID, KC_BRIU, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_APP, KC_SLCK, KC_PAUSE, KC_NO, KC_NO,
		KC_TRNS, KC_NO, KC_RCTL, KC_RALT, KC_RGUI, KC_RSFT, KC_NO,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
};

