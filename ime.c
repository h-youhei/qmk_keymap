#include "ime.h"
#include "kana.h" //is_kana, need_commit, register_kana
#include "kana_chord.h" //is_kana_chord, process_kana_chord
#include "util_user.h" //in_range

#include "action_layer.h" //default_layer_state
#include "action_util.h" //keyboard_report, send_keyboard_report, add/del_weak_mods
#include "host.h" //IS_HOST_LED_ON/OFF
#include "keymap_jp.h" //JP_*
#include "led.h" //USB_LED_*
#include "util.h" //biton32

static void numlock_on(void);
static void numlock_off(void);
static bool is_commit_mode(void);
static void convert_sequence(void);
static void predict_sequence(void);

static uint8_t im_state = IM_STATE_PRECOMPOSITION;
uint8_t get_im_state() { return im_state; }

static bool _is_practice_mode = false;
bool is_practice_mode() { return _is_practice_mode; }

void reset_ime() {
	im_state = IM_STATE_PRECOMPOSITION;
	_is_practice_mode = false;
	numlock_off();
}

static bool is_commit_mode() {
	return (im_state == IM_STATE_HIRAGANA_DIRECT || im_state == IM_STATE_KATAKANA_DIRECT);
}

bool is_default_layer_kana() {
	return (biton32(default_layer_state) == LAYER_KANA);
}

// TODO: use raw_hid to detect ime state
static void numlock_on() {
	if(IS_HOST_LED_OFF(USB_LED_NUM_LOCK)) {
		tap_code(KC_NLCK);
	}
}
static void numlock_off() {
	if(IS_HOST_LED_ON(USB_LED_NUM_LOCK)) {
		tap_code(KC_NLCK);
	}
}

// TODO: use raw_hid to detect ime state
void detect_ime_change(uint8_t usb_led) {
	// use Num_Lock to recognize that
	// modal editor change ime state
	if(IS_LED_ON(usb_led, USB_LED_NUM_LOCK)) {
		default_layer_set(1UL << LAYER_KANA);
	}
	else {
		default_layer_set(1UL << L_BASE);
		if(_is_practice_mode) return;
		switch(im_state) {
		case IM_STATE_CONVERT:
		case IM_STATE_COMPOSITION:
		case IM_STATE_PREDICT:
			im_state = IM_STATE_PRECOMPOSITION;
			break;
		default:
			break;
		}
	}
}

__attribute__((weak))
bool need_commit(uint16_t keycode) {
	switch(keycode) {
		case KANA_COMM:
		case KANA_DOT:
		case KANA_ZSLSH:
		case KANA_ZDOT:
		case KANA_ZCOMM:
			return true;
		default:
			return false;
	}
}

void tap_kana(uint16_t kana, keyevent_t event) {
	process_kana(kana, &(keyrecord_t){
		// .tap = (tap_t){},
		.event.key = event.key,
		.event.time = event.time,
		.event.pressed = true
	});
}

static void convert_sequence(void) {
	tap_code(KC_SPC);
	// to show up candidates
	tap_code(KC_SPC);
	// select first candidate
	tap_code(KC_1);
}
static void predict_sequence(void) {
	tap_code(KC_TAB);
	// to show up candidates
	tap_code(KC_TAB);
	// select first candidate
	// KC_1 is broken if there is no candidate
	tap_code(KC_UP);
}

// return value
// true: continue processing
// false: The process is completed by this function
bool process_kana(uint16_t keycode, keyrecord_t *record) {
	if(!is_kana(keycode)) return true;
	keyevent_t event = record->event;
	if(!event.pressed) return false;
	if(keyboard_report->mods || IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
		tap_code(keymap_key_to_keycode(0, event.key));
		return false;
	}
	if(_is_practice_mode) {
		register_kana(keycode);
	}
	else if(need_commit(keycode)) {
	// for punctuation character
	// PreComposition: char, commit
	// Convert: cansel, char, commit
	// Predict: (commit by ime), char, commit
		if(im_state == IM_STATE_CONVERT) {
			tap_code(KC_ESC);
		}
		register_kana(keycode);
		tap_code(KC_ENT);
		im_state = IM_STATE_PRECOMPOSITION;
	}
	else {
	// for normal character
	// PreComposition: char, convert
	// Convert: cansel, char, convert
	// Predict: commit(by ime), char, convert
		if(im_state == IM_STATE_CONVERT) {
			tap_code(KC_ESC);
		}
		register_kana(keycode);
		if(is_commit_mode()) {
			tap_code(KC_ENT);
		}
		else {
			convert_sequence();
			im_state = IM_STATE_CONVERT;
		}
	}
	return false;
}

// return value
// true: continue processing
// false: The process is completed by this function
bool process_ime(uint16_t keycode, keyrecord_t *record) {
	keyevent_t event = record->event;
	if(keycode == IME) {
		if(!event.pressed) return false;
		if(is_default_layer_kana()) {
			// IME: off to on, practice: on to off
			if(_is_practice_mode) {
				_is_practice_mode = false;
				// to update led indicator
				default_layer_state_set_user(default_layer_state);
			}
			// IME: on to off, practice: off
			else {
				// TODO: use raw_hid to detect ime state
				numlock_off();
			}
		}
		// IME: off to on, practice: off
		else {
			// TODO: use raw_hid to detect ime state
			numlock_on();
		}
		tap_code(JP_ZHTG);
		return false;
	}
	if(keycode == PRACTICE_MODE) {
		if(!event.pressed) return false;
		if(is_default_layer_kana()) {
			// IME: off, practice: on to off
			if(_is_practice_mode) {
				// TODO: use raw_hid to detect ime state
				numlock_off();
				// should be after numlock_off
				_is_practice_mode = false;
			}
			// IME: on to off, practice: off to on
			else {
				tap_code(JP_ZHTG);
				_is_practice_mode = true;
				// to update led indicator
				default_layer_state_set_user(default_layer_state);
			}
		}
		// IME: off, practice: off to on
		else {
			_is_practice_mode = true;
			numlock_on();
		}
		return false;
	}
	if(biton32(default_layer_state) != LAYER_KANA) return true;
	if(is_kana_chord(keycode)) {
		process_kana_chord(keycode, record);
		return false;
	}
	if(is_kana(keycode)) {
		process_kana(keycode, record);
		return false;
	}
	if(keyboard_report->mods || IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
		return true;
	}
	// nothing to do in practice mode after this line
	if(_is_practice_mode) return true;
	if(in_range(keycode, KC_1, KC_0)) {
	// Predict: select candidate, commit
	// Convert: select candidate, commit
	// PreComposition etc: num char, commit
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_PREDICT:
		case IM_STATE_CONVERT:
			tap_code(keycode);
			tap_code(KC_ENT);
			im_state = IM_STATE_PRECOMPOSITION;
			break;
		default:
			tap_code(keycode);
			tap_code(KC_ENT);
			break;
		}
		return false;
	}
	switch(keycode) {
	case KC_MINS:
	// PreComposition: char
	// Convert: cansel, char, convert
	// Predict: commit(by ime), char
		if(!event.pressed) return false;
		if(im_state == IM_STATE_CONVERT) {
			tap_code(KC_ESC);
			tap_code(keycode);
			convert_sequence();
		}
		else {
			tap_code(keycode);
			if(is_commit_mode()) {
				tap_code(KC_ENT);
			}
			else { im_state = IM_STATE_COMPOSITION; }
		}
		return false;
	case IM_HIRAGANA:
	// PreComposition: turn on hiragana mode
	// HiraganaDirect: turn off hiragana mode
	// KatakanaDirect: turn on hiragana mode
	// Predict: hiragana, commit
	// Convert: hiragana, commit
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_PRECOMPOSITION:
		case IM_STATE_KATAKANA_DIRECT:
			tap_code(KC_KANA);
			im_state = IM_STATE_HIRAGANA_DIRECT;
			break;
		case IM_STATE_HIRAGANA_DIRECT:
			im_state = IM_STATE_PRECOMPOSITION;
			break;
		case IM_STATE_COMPOSITION:
		case IM_STATE_CONVERT:
			tap_code(KC_F6);
			tap_code(KC_ENT);
			im_state = IM_STATE_PRECOMPOSITION;
			break;
		default:
			break;
		}
		return false;
	case IM_KATAKANA:
	// PreComposition: turn on katakana mode
	// HiraganaDirect: turn on katakana mode
	// KatakanaDirect: turn off katakana mode
	// Predict: katakana, commit
	// Convert: katakana, commit
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_PRECOMPOSITION:
		case IM_STATE_HIRAGANA_DIRECT:
			add_weak_mods(MOD_LSFT);
			send_keyboard_report();
			tap_code(KC_KANA);
			del_weak_mods(MOD_LSFT);
			send_keyboard_report();
			im_state = IM_STATE_KATAKANA_DIRECT;
			break;
		case IM_STATE_KATAKANA_DIRECT:
			tap_code(KC_KANA);
			im_state = IM_STATE_PRECOMPOSITION;
			break;
		case IM_STATE_COMPOSITION:
		case IM_STATE_CONVERT:
			tap_code(KC_F7);
			tap_code(KC_ENT);
			im_state = IM_STATE_PRECOMPOSITION;
			break;
		default:
			break;
		}
		return false;
	case IM_LSPC:
	// PreComposition etc: space
	// Composition: commit
	// Predict, Convert: hiragana, commit
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_COMPOSITION:
			tap_code(KC_ENT);
			im_state = IM_STATE_PRECOMPOSITION;
			break;
		case IM_STATE_CONVERT:
		case IM_STATE_PREDICT:
			tap_code(KC_ESC);
			tap_code(KC_ENT);
			im_state = IM_STATE_PRECOMPOSITION;
			break;
		default:
			tap_code(KC_SPC);
			break;
		}
		return false;
	case IM_RSPC:
	// PreComposition etc: space
	// Composition: convert
	// Predict, Convert: commit
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_COMPOSITION:
			convert_sequence();
			im_state = IM_STATE_CONVERT;
		case IM_STATE_CONVERT:
		case IM_STATE_PREDICT:
			tap_code(KC_ENT);
			im_state = IM_STATE_PRECOMPOSITION;
			break;
		default:
			tap_code(KC_SPC);
			break;
		}
		return false;
	case IM_LENT:
	// PreComposition etc: return
	// Composition: commit, return
	// Convert, Predict: hiragana, commit, return
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_COMPOSITION:
			tap_code(KC_ENT);
			tap_code(KC_ENT);
			im_state = IM_STATE_PRECOMPOSITION;
			break;
		case IM_STATE_CONVERT:
		case IM_STATE_PREDICT:
			tap_code(KC_ESC);
			tap_code(KC_ENT);
			tap_code(KC_ENT);
			im_state = IM_STATE_PRECOMPOSITION;
			break;
		default:
			tap_code(KC_ENT);
			break;
		}
		return false;
	case IM_RENT:
	// PreComposition etc: return
	// Composition, Convert, Predict: commit, return
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_COMPOSITION:
		case IM_STATE_CONVERT:
		case IM_STATE_PREDICT:
			tap_code(KC_ENT);
			tap_code(KC_ENT);
			im_state = IM_STATE_PRECOMPOSITION;
			break;
		default:
			tap_code(KC_ENT);
			break;
		}
		return false;
	case KC_TAB:
		// Composition, Convert: predict
		// Predict: next candidate
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_COMPOSITION:
			predict_sequence();
			im_state = IM_STATE_PREDICT;
			break;
		case IM_STATE_CONVERT:
			tap_code(KC_ESC);
			predict_sequence();
			im_state = IM_STATE_PREDICT;
			break;
		default:
			tap_code(KC_TAB);
			break;
		}
		return false;
	case KC_ESC:
		// Composition, Predict: cansel
		// Convert: cansel, delete all
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_COMPOSITION:
			tap_code(KC_ESC);
			im_state = IM_STATE_PRECOMPOSITION;
			break;
		case IM_STATE_CONVERT:
			tap_code(KC_ESC);
			tap_code(KC_ESC);
			im_state = IM_STATE_PRECOMPOSITION;
			break;
		case IM_STATE_PREDICT:
			tap_code(KC_ESC);
			im_state = IM_STATE_COMPOSITION;
			break;
		default:
			tap_code(KC_ESC);
			break;
		}
		return false;
	case KC_BSPC:
	case KC_DEL:
	// Predict: cansel, delete char, predict
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_CONVERT:
			tap_code(KC_ESC);
			tap_code(keycode);
			im_state = IM_STATE_COMPOSITION;
			// broken when only one letter is left
			// convert_sequence();
			break;
		case IM_STATE_PREDICT:
			tap_code(KC_ESC);
			tap_code(keycode);
			im_state = IM_STATE_COMPOSITION;
			// broken when only one letter is left
			// tap_code(KC_TAB);
			break;
		default:
			tap_code(keycode);
			break;
		}
		return false;
	case KC_HOME:
	case KC_END:
	// Predict: cansel, move cursor, predict
	// Convert: move segment
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_PREDICT:
			tap_code(KC_ESC);
			tap_code(keycode);
			im_state = IM_STATE_COMPOSITION;
			break;
		default:
			tap_code(keycode);
			break;
		}
		return false;
	case KC_LEFT:
	case KC_RGHT:
	// Composition, Convert: expand/shrink segment
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_COMPOSITION:
			add_weak_mods(MOD_LSFT);
			send_keyboard_report();
			tap_code(keycode);
			del_weak_mods(MOD_LSFT);
			send_keyboard_report();
			convert_sequence();
			im_state = IM_STATE_CONVERT;
			break;
		case IM_STATE_CONVERT:
			add_weak_mods(MOD_LSFT);
			send_keyboard_report();
			tap_code(keycode);
			del_weak_mods(MOD_LSFT);
			send_keyboard_report();
			convert_sequence();
			break;
	// Predict: cansel, move cursor, predict
		default:
			tap_code(keycode);
			break;
		}
		return false;
	case KC_UP:
	// Composition, Convert: select next segment
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_COMPOSITION:
			tap_code(KC_RGHT);
			convert_sequence();
			im_state = IM_STATE_CONVERT;
			break;
		case IM_STATE_CONVERT:
			tap_code(KC_RGHT);
			convert_sequence();
			break;
		default:
			tap_code(keycode);
			break;
		}
		return false;
	case KC_DOWN:
	// Composition, Convert: select prev segment
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_COMPOSITION:
			tap_code(KC_LEFT);
			convert_sequence();
			im_state = IM_STATE_CONVERT;
			break;
		case IM_STATE_CONVERT:
			tap_code(KC_LEFT);
			convert_sequence();
			break;
		default:
			tap_code(keycode);
			break;
		}
		return false;
	case KC_HENK:
		if(!event.pressed) return false;
		tap_code(keycode);
		if(im_state == IM_STATE_COMPOSITION) {
			im_state = IM_STATE_CONVERT;
		}
		return false;
	}
	return true;
}
