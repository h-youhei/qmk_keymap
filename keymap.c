#include "ergodox.h"

enum Layer {
	L_BASE = 0,
	L_SYMBOL,
	L_ARROW,
	L_WORKSPACE,
};

// reset layer state
#define CLEAR M(0)

// Tap or Layer
//#define SYMB_T(kc) LT(L_SYMBOL, kc)
#define SYMB_T(kc) LT(L_SYMBOL,kc)
#define ARRW_T(kc) LT(L_ARROW, kc)
#define FN_T(kc) LT(L_ARROW, kc)
#define WS_T(kc) LT(L_WORKSPACE, kc)

#define SFT_OS OSM(MOD_LSFT)
#define KC_SREQ KC_SYSREQ

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[L_BASE] = KEYMAP(
		// left hand
		CLEAR, KC_NO, KC_GRV, KC_LBRC, KC_RBRC, KC_NO, KC_NO,
		KC_NO, KC_J, KC_Y, KC_L, KC_R, KC_B, SFT_T(KC_ENT),
		KC_Z, KC_K, KC_I, KC_T, KC_N, KC_M,
		KC_BSLS, KC_SLSH, KC_MINS, KC_F, KC_H, KC_V, WS_T(KC_PGUP),
		KC_NO, KC_NO, KC_NO, KC_MUTE, KC_MPLY,
		// thumb
		ALT_T(KC_HOME), CTL_T(KC_END),
		GUI_T(KC_ESC),
		SFT_OS, SYMB_T(KC_SPC), ARRW_T(KC_MHEN),
		// right hand
		KC_NO, KC_NO, KC_BSPC, KC_DEL, KC_INS, KC_NO, KC_NO,
		SFT_T(KC_ENT), KC_W, KC_C, KC_O, KC_U, KC_Q, KC_NO,
		KC_P, KC_S, KC_E, KC_A, KC_QUOT, KC_X,
		WS_T(KC_PGDN), KC_G, KC_D, KC_COMM, KC_DOT, KC_EQL, KC_SCLN,
		KC_PSCR, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		CTL_T(KC_CAPS), ALT_T(KC_KANA),
		GUI_T(KC_TAB),
		FN_T(KC_PAUS), SYMB_T(KC_SPC), SFT_OS

	),
	[L_SYMBOL] = KEYMAP(
		// left hand
		KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_PERC, KC_AMPR, KC_ASTR, KC_CIRC, KC_TRNS,
		KC_NO, KC_NO, KC_AT, KC_LPRN, KC_RPRN, KC_DLR,
		KC_NO, KC_NO, KC_NO, KC_EXLM, KC_HASH, KC_NO, KC_TRNS,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO, RESET,
		KC_TRNS, KC_NO, KC_4, KC_5, KC_6, KC_NO, KC_NO,
		KC_0, KC_1, KC_2, KC_3, KC_NO, KC_NO,
		KC_TRNS, KC_NO, KC_7, KC_8, KC_9, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_ARROW] = KEYMAP(
		// left hand
		KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_F4, KC_F3, KC_F2, KC_F1, KC_NO, KC_TRNS,
		KC_NO, KC_F8, KC_F7, KC_F6, KC_F5, KC_NO,
		KC_NO, KC_F12, KC_F11, KC_F10, KC_F9, KC_NO, KC_TRNS,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO, RESET,
		KC_TRNS, KC_NO, KC_LEFT, KC_RGHT, KC_APP, KC_NO, KC_NO,
		KC_NO, KC_DOWN, KC_UP, LALT(KC_NO), KC_NO, KC_NO,
		KC_TRNS, KC_NO, LALT(KC_DOWN), LALT(KC_UP), KC_SLCK, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_WORKSPACE] = KEYMAP(
		// left hand
		KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, LGUI(KC_9), LGUI(KC_8), KC_NO, KC_TRNS,
		KC_NO, KC_NO, LGUI(KC_7), LGUI(KC_6), LGUI(KC_5), KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RESET,
		KC_TRNS, KC_NO, LGUI(KC_3), LGUI(KC_4), KC_NO, KC_NO, KC_NO,
		KC_NO, LGUI(KC_0), LGUI(KC_1), LGUI(KC_2), KC_NO, KC_NO,
		KC_TRNS, KC_NO, LGUI(KC_PGDN), LGUI(KC_PGUP), KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
};


const uint16_t PROGMEM fn_actions[] = {};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
	switch(id) {
		case 0:
			if(record->event.pressed) {
				layer_clear();
			}
			break;
	}
	return MACRO_NONE;
};

void matrix_init_user(void) {};

void matrix_scan_user(void) {};
