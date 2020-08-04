#ifndef ime_h_INCLUDED
#define ime_h_INCLUDED

#include "action.h" //keyrecord_t
#include "kana.h" //IME_RANGE

#if !defined(LAYER_KANA)
#warning "LAYER_KANA is not defined. It is set to default value 1"
#define LAYER_KANA 1
#endif

uint8_t get_im_state(void);
bool is_practice_mode(void);
bool is_default_layer_kana(void);
void reset_ime(void);

void detect_ime_change(uint8_t usb_led);

// You can change what character to commit after input
// by overriding this function
bool need_commit(uint16_t keycode);

// return value
// true: continue processing
// false: The process is completed by this function
bool process_kana(uint16_t keycode, keyrecord_t *record);

void tap_kana(uint16_t keycode, keyevent_t event);

// return value
// true: continue processing
// false: The process is completed by this function
bool process_ime(uint16_t keycode, keyrecord_t *record);

enum im_state {
	IM_STATE_DIRECT,
	IM_STATE_HIRAGANA_DIRECT,
	IM_STATE_KATAKANA_DIRECT,
	IM_STATE_PRECOMPOSITION,
	IM_STATE_COMPOSITION,
	IM_STATE_CONVERT,
	IM_STATE_PREDICT,
};

enum im_code {
	IME = IME_RANGE,
	PRACTICE_MODE,
	IM_HIRAGANA,
	IM_KATAKANA,
	IM_COMMIT,
	IM_LSPC,
	IM_RSPC,
	IM_LENT,
	IM_RENT,
};


#endif // ime_h_INCLUDED
