#ifndef ime_h_INCLUDED
#define ime_h_INCLUDED

#include "action.h" //keyrecord_t
#include "kana.h" //IME_RANGE

#if !defined(LAYER_KANA)
#warning "LAYER_KANA is not defined. It is set to default value 1"
#define LAYER_KANA 1
#endif

bool is_practice_mode(void);
bool is_default_layer_kana(void);
void reset_ime(void);

void on_im_off(void);
void on_im_on(void);

// return value
// true: continue processing
// false: The process is completed by this function
bool process_capital_letter(uint16_t keycode, keyrecord_t *record);
bool process_kana(uint16_t keycode, keyrecord_t *record);

void tap_kana(uint16_t keycode, keyevent_t event);

// return value
// true: continue processing
// false: The process is completed by this function
bool process_ime(uint16_t keycode, keyrecord_t *record);

uint8_t get_im_state(void);
void im_state_set_user(uint8_t im_state);
enum im_state {
	IM_STATE_HIRAGANA_DIRECT,
	IM_STATE_KATAKANA_DIRECT,
	IM_STATE_PRECOMPOSITION,
	IM_STATE_COMPOSITION,
	IM_STATE_CONVERT,
	IM_STATE_PREDICT,
};

#define HIRAGANA KC_F6
#define KATAKANA KC_F7

enum im_code {
	IME = IME_RANGE,
	PRACTICE_MODE,
};

// You can change behavior when character is typed
// by overriding this function.
// COMMIT: commit after input
// COMMIT_BEFORE: commit before input
// HALFWIDTH: commit before input to prevent
//  already typed characters from being halfwidth.
//  then convert to halfwidth. finally commit.
uint8_t get_behavior(uint16_t keycode);
enum behavior {
	BEHAVE_NORMAL,
	BEHAVE_COMMIT,
	BEHAVE_COMMIT_BEFORE,
	BEHAVE_COMMIT_ONLY_PRECOMPOSITION,
	BEHAVE_HALFWIDTH,
};
// Here are symbols to change behavior
// JP_GRV
// JP_TILD
// JP_EXLM
// JP_AT
// JP_HASH
// JP_DLR
// JP_PERC
// JP_CIRC
// JP_AMPR
// JP_ASTR
// JP_LPRN
// JP_RPRN
// KC_MINS
// JP_UNDS
// JP_EQL
// JP_PLUS
// JP_LBRC
// JP_LCBR
// JP_RBRC
// JP_RCBR
// JP_BSLS
// JP_PIPE
// KC_SCLN
// JP_COLN
// JP_QUOT
// JP_DQT
// KC_COMM
// JP_LT
// KC_DOT
// JP_GT
// KC_SLSH
// JP_QUES

#endif // ime_h_INCLUDED
