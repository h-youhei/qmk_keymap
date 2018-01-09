#include "ergodox_ez.h"
#include "util_user.h"
#include "process_simultaneous.h"

enum user_code {
  KC_P000 = SAFE_RANGE,
};

enum Layer {
  L_BASE,
  #ifdef ENABLE_STABLE_LAYER
  L_STABLE,
  #endif
  L_FN,
};
#define FN_T(kc) LT(L_FN, kc)
#define FN_S(kc) LS(L_FN, kc)

// click (left, middle, right)
#define KC_LCLK KC_BTN1
#define KC_MCLK KC_BTN3
#define KC_RCLK KC_BTN2

#ifdef ENABLE_STABLE_LAYER
#define STABLE DF(L_STABLE)
#else
#define STABLE KC_NO
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
  if(usb_led & (1<<USB_LED_CAPS_LOCK)) ergodox_right_led_2_on();
  else ergodox_right_led_2_off();

  if(!(usb_led & (1<<USB_LED_NUM_LOCK))) {
	register_code(KC_NUMLOCK);
	unregister_code(KC_NUMLOCK);
  }
}

/* TODO: open this function in upstream. */
#ifdef ENABLE_STABLE_LAYER
void default_layer_state_set_user(uint32_t state) {
  if(state == L_STABLE) ergodox_right_led_1_on();
  else ergodox_right_led_1_off();
}
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_BASE] = KEYMAP(
		// left hand
		KC_PSCR, KC_4, KC_3, KC_2, KC_1, KC_5, KC_NO,
		KC_MHEN, KC_Q, KC_C, KC_L, KC_P, KC_V, KC_ESC,
		KC_SLSH, KC_O, KC_S, KC_R, KC_N, KC_H,
		KC_MCLK, KC_X, KC_J, KC_MINS, KC_F, KC_B, KC_BSPC,
		KC_RCLK, KC_LCLK, LGUI_S(KC_GRV), LALT_S(KC_LBRC), LCTL_S(KC_RBRC),
		// thumb
		KC_NO, KC_NO,
		KC_NO,
		LSFT_S(KC_SPC), FN_S(KC_ENT), KC_CAPS,
		// right hand
		KC_NO, KC_8, KC_9, KC_0, KC_6, KC_7, STABLE,
		KC_INS, KC_K, KC_G, KC_U, KC_Y, KC_Z, KC_HENK,
		KC_D, KC_T, KC_E, KC_I, KC_A, KC_SCLN,
		KC_DEL, KC_W, KC_M, KC_COMM, KC_DOT, KC_QUOT, KC_LCLK,
		RCTL_S(KC_EQL), RALT_S(KC_BSLS), RGUI_S(KC_PAUS), KC_MCLK, KC_RCLK,
		// thumb
		KC_NO, KC_NO,
		KC_NO,
		KC_KANA, FN_S(KC_TAB), RSFT_S(KC_SPC)
	),
  #ifdef ENABLE_STABLE_LAYER
	[L_STABLE] = KEYMAP(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, LGUI_T(KC_GRV), LALT_T(KC_LBRC), LCTL_T(KC_RBRC),
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		LSFT_T(KC_SPC), FN_T(KC_ENT), KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, DF(L_BASE),
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		RCTL_T(KC_EQL), RALT_T(KC_BSLS), RGUI_T(KC_PAUS), KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, FN_T(KC_TAB), RSFT_T(KC_SPC)
	),
  #endif
	[L_FN] = KEYMAP(
		// left hand
		KC_NO, KC_F4, KC_F3, KC_F2, KC_F1, KC_F5, KC_NO,
		KC_NO, KC_NO, KC_P6, KC_P5, KC_P4, KC_NO, KC_NO,
		KC_NO, KC_PDOT, KC_P3, KC_P2, KC_P1, KC_P0,
		KC_MUTE, KC_NO, KC_P9, KC_P8, KC_P7, KC_P000, KC_NO,
		KC_VOLD, KC_VOLU, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_NO, KC_NO,
		KC_NO,
		KC_TRNS, KC_TRNS, KC_NO,
		// right hand
		KC_NO, KC_F8, KC_F9, KC_F10, KC_F6, KC_F7, KC_NO,
		KC_NO, KC_NO, KC_LEFT, KC_RGHT, KC_F11, KC_F12, KC_NO,
		KC_HOME, KC_DOWN, KC_UP, KC_END, KC_APP, KC_NO,
		KC_NO, KC_NO, KC_PGDN, KC_PGUP, KC_NO, KC_NO, KC_NO,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO,
		// thumb
		KC_NO, KC_NO,
		KC_NO,
		KC_NO, KC_TRNS, KC_TRNS
	),
};
