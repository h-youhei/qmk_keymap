/*TODO
 */
#include "ergodox.h"
#include "action_layer.h"
#include "debug.h"


enum Layer {
	L_BASE = 0,
	L_SYMBOL,
	L_ARROW,
};
#define L_NUMBER L_SYMBOL
// for Window Manager
#define L_WORKSPACE L_ARROW

// reset layer state
#define BASE M(0)

// Tap or Layer
#define ARRW_T(kc) LT(L_ARROW, kc)
#define SYMB_T(kc) LT(L_SYMBOL, kc)
#define NUM_T(kc) LT(L_NUMBER, kc)
#define WS_T(kc) LT(L_WORKSPACE, kc)


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[L_BASE] = KEYMAP(
		// left hand
		BASE, KC_NO, KC_CAPS, KC_LBRC, KC_RBRC, KC_GRV, KC_NO,
		KC_PAUS, KC_J, KC_Y, KC_O, KC_C, KC_P, SFT_T(KC_F11),
		KC_Q, KC_K, KC_I, KC_E, KC_S, KC_W,
		KC_SCLN, KC_QUOT, KC_COMM, KC_DOT, KC_D, KC_G, KC_F12,
		KC_F10, KC_F9, KC_F8, KC_F7, KC_F6,
		// thumb
		ARRW_T(KC_ESC), CTL_T(KC_SLCK),
		KC_LGUI,
		NUM_T(KC_ENT), SFT_T(KC_SPC), ALT_T(KC_HOME),
		// right hand
		KC_NO, KC_INS, KC_BSPC, KC_DEL, KC_KANA, KC_NO, KC_NO,
		SFT_T(KC_MPLY), KC_F, KC_R, KC_L, KC_U, KC_X, KC_PSCR,
		KC_M, KC_N, KC_T, KC_A, KC_V, KC_Z,
		KC_MUTE, KC_B, KC_H, KC_MINS, KC_EQL, KC_SLSH, KC_BSLS,
		KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,
		// thumb
		CTL_T(KC_PGUP), WS_T(KC_TAB),
		GUI_T(KC_PGDN),
		ALT_T(KC_END), SFT_T(KC_SPC),SYMB_T(KC_ENT)
	),
	[L_SYMBOL] = KEYMAP(
		// left hand
		KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_PERC, KC_ASTR, KC_AMPR, KC_CIRC, KC_TRNS,
		KC_NO, KC_NO, KC_AT, KC_LPRN, KC_RPRN, KC_DLR,
		KC_NO, KC_NO, KC_NO, KC_EXLM, KC_HASH, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO, RESET,
		KC_TRNS, KC_NO, KC_4, KC_5, KC_6, KC_NO, KC_NO,
		KC_0, KC_1, KC_2, KC_3, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_7, KC_8, KC_9, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_ARROW] = KEYMAP(
		// left hand
		KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_APP, LALT(KC_NO), KC_NO, KC_TRNS,
		KC_NO, LGUI(KC_4), LGUI(KC_3), LGUI(KC_2), LGUI(KC_1), KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RESET,
		KC_TRNS, KC_NO, KC_LEFT, KC_RGHT, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_DOWN, KC_UP, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
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
