#include "ergodox.h"

enum Layer {
	L_BASE = 0,
	L_SYMBOL,
	L_ARROW,
	L_NUM,
	L_FUNCTION,
};

// reset layer state
#define CLEAR M(0)

// Tap or Layer
//#define SYMB_T(kc) LT(L_SYMBOL, kc)
#define SYMB_T(kc) LT(L_SYMBOL,kc)
#define ARRW_T(kc) LT(L_ARROW, kc)
#define NUM_T(kc) LT(L_NUM, kc)
#define FN_T(kc) LT(L_FUNCTION, kc)

#define SFT_OS OSM(MOD_LSFT)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[L_BASE] = KEYMAP(
		// left hand
		CLEAR, KC_NO, KC_GRV, KC_LBRC, KC_RBRC, KC_NO, KC_NO,
		KC_NO, KC_J, KC_Y, KC_L, KC_R, KC_B, KC_ESC,
		KC_Q, KC_K, CTL_T(KC_I), ALT_T(KC_T), GUI_T(KC_N), KC_H,
		KC_BSLS, KC_SLSH, KC_MINS, KC_F, KC_M, KC_V, KC_NO,
		KC_NO, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI,
		// thumb
		KC_NO, KC_NO,
		KC_NO,
		ARRW_T(KC_SPC), SFT_OS, NUM_T(KC_ENT),
		// right hand
		KC_NO, KC_NO, KC_BSPC, KC_DEL, KC_INS, KC_NO, KC_NO,
		KC_TAB, KC_W, KC_C, KC_O, KC_U, KC_Z, KC_NO,
		KC_P, GUI_T(KC_S), ALT_T(KC_E), CTL_T(KC_A), KC_X, KC_QUOT,
		KC_NO, KC_G, KC_D, KC_COMM, KC_DOT, KC_EQL, KC_SCLN,
		KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT, KC_NO,
		// thumb
		KC_NO, KC_NO,
		KC_NO,
		FN_T(KC_ENT), SFT_OS, SYMB_T(KC_SPC)
	),
	[L_SYMBOL] = KEYMAP(
		// left hand
		KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_PERC, KC_AMPR, KC_ASTR, KC_CIRC, KC_TRNS,
		KC_NO, KC_NO, KC_AT, KC_LPRN, KC_RPRN, KC_DLR,
		KC_NO, KC_NO, KC_NO, KC_EXLM, KC_HASH, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RESET,
		KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, GUI_T(KC_KANA), ALT_T(KC_CAPS), CTL_T(KC_PAUS), KC_NO, KC_NO,
		KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_ARROW] = KEYMAP(
		// left hand
		KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS,
		KC_NO, KC_NO, CTL_T(KC_PSCR), ALT_T(KC_MUTE), GUI_T(KC_MPLY), KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, RESET,
		KC_TRNS, KC_NO, KC_DOWN, KC_UP, KC_APP, KC_NO, KC_NO,
		KC_HOME, KC_LEFT, KC_RGHT, KC_END, KC_NO, KC_NO,
		KC_TRNS, KC_NO, KC_PGDN, KC_PGUP, KC_SLCK, KC_NO, KC_NO,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_NUM] = KEYMAP(
		// left hand
		KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS,
		KC_NO, KC_NO, CTL_T(KC_NO), ALT_T(KC_NO), GUI_T(KC_NO), KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_NO, KC_NO, KC_7, KC_8, KC_9, KC_NO, RESET,
		KC_TRNS, KC_NO, KC_4, KC_5, KC_6, KC_NO, KC_NO,
		KC_NO, KC_1, KC_2, KC_3, KC_NO, KC_NO,
		KC_TRNS, KC_NO, KC_0, KC_TRNS, KC_TRNS, KC_NO, KC_NO,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_FUNCTION] = KEYMAP(
		// left hand
		KC_TRNS, KC_NO, KC_F9, KC_F8, KC_F7, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_F6, KC_F5, KC_F4, KC_NO, KC_TRNS,
		KC_NO, KC_NO, KC_F3, KC_F2, KC_F1, KC_NO,
		KC_NO, KC_NO, KC_F12, KC_F11, KC_F10, KC_NO, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RESET,
		KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, GUI_T(KC_NO), ALT_T(KC_NO), CTL_T(KC_NO), KC_NO, KC_NO,
		KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
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
				clear_mods();
				clear_oneshot_mods();
			}
			break;
	}
	return MACRO_NONE;
};

void matrix_init_user(void) {};

void matrix_scan_user(void) {};
