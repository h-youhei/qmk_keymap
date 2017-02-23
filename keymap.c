#include "youhei.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[L_BASE] = KEYMAP(
		// left hand
		KC_PSCR, KC_NO, KC_GRV, KC_LBRC, KC_RBRC, KC_MPLY, KC_MUTE,
		KC_NO, KC_J, KC_U, KC_O, KC_P, KC_SLSH, KC_BSLS,
		KC_QUOT, LCTL_T(KC_W), LALT_T(KC_A), LGUI_T(KC_E), DIR_T(KC_I), NUM_T(KC_Y),
		KC_LALT, KC_Z, KC_COMM, KC_DOT, KC_B, KC_MINS, KC_NO,
		KC_LCTL, KC_LGUI, KC_RCLK, KC_MCLK, KC_LCLK,
		// thumb
		KC_NO, KC_NO,
		KC_CMPS,
		LSFT_T(KC_SPC), LRPT_T(KC_ENT), KC_ESC,
		// right hand
		KC_VOLD, KC_VOLU, KC_BSPC, KC_DEL, KC_INS, KC_NO, KC_PAUS,
		KC_EQL, KC_V, KC_M, KC_L, KC_C, KC_X, KC_NO,
		FN_T(KC_H), SIGN_T(KC_R), RGUI_T(KC_T), ALT_T(KC_S), RCTL_T(KC_F), KC_SCLN,
		KC_NO, KC_K, KC_N, KC_G, KC_D, KC_Q, KC_LALT,
		KC_LCLK, KC_MCLK, KC_RCLK, KC_RGUI, KC_RCTL,
		// thumb
		KC_NO, KC_NO,
		KC_TGIM,
		KC_CAPS, RRPT_T(KC_TAB), RSFT_T(KC_SPC)
	),
	[L_LREPEAT] = KEYMAP(
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
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_H, KC_R, KC_T, KC_S, KC_F, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_MCLK, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TAB, KC_SPC
	),
	[L_RREPEAT] = KEYMAP(
		// left hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_W, KC_A, KC_E, KC_I, KC_Y,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_MCLK, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_SPC, KC_ENT, KC_TRNS,
		// right hand
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_SIGN] = KEYMAP(
		// left hand
		CLEAR, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_PERC, KC_AMPR, KC_ASTR, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_DLR, KC_LPRN, KC_RPRN, KC_CIRC,
		KC_NO, KC_NO, KC_AT, KC_HASH, KC_EXLM, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RESET,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_DIRECTION] = KEYMAP(
		// left hand
		CLEAR, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, RESET,
		KC_NO, KC_HOME, KC_PGDN, KC_PGUP, KC_END, KC_NO, KC_NO,
		KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_APP, KC_SLCK,
		KC_NO, KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_NUMBER] = KEYMAP(
		// left hand
		CLEAR, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_TRNS,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, RESET,
		KC_NO, KC_NO, KC_4, KC_5, KC_6, KC_NO, KC_NO,
		KC_0, KC_1, KC_2, KC_3, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_7, KC_8, KC_9, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
	[L_FUNCTION] = KEYMAP(
		// left hand
		CLEAR, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_F6, KC_F5, KC_F4, KC_F11, KC_NO,
		KC_NO, KC_NO, KC_F3, KC_F2, KC_F1, KC_F10,
		KC_NO, KC_NO, KC_F9, KC_F8, KC_F7, KC_F12, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,
		// right hand
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RESET,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_TRNS, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		// thumb
		KC_TRNS, KC_TRNS,
		KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS
	),
};
