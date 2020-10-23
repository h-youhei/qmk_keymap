#include "ime.h"
#include "kana.h" //is_kana, need_commit, register_kana
#include "kana_chord.h" //is_kana_chord, process_kana_chord
#include "util_user.h" //in_range, is_shifting_but_other_mod, tap_shifted_code, tap_code_unmods

#include "action_layer.h" //default_layer_state
#include "action_util.h" //keyboard_report, send_keyboard_report, add/del_weak_mods
#include "host.h" //IS_HOST_LED_ON/OFF
#include "keymap_jp.h" //JP_*
#include "led.h" //USB_LED_*
#include "util.h" //biton32

static bool is_commit_mode(void);
static void set_im_state(uint8_t state);
static void handle_cursor(uint16_t keycode);
static void reset_cursor(void);
static void numlock_on(void);
static void numlock_off(void);
static void capslock_off(void);
static void convert_sequence(void);
static void predict_sequence(void);

static bool is_ascii_symbol(uint16_t keycode);
static bool is_shifted_symbol_jp(uint16_t keycode);
static void tap_code_handle_shifted_jp(uint16_t keycode);
static void commit_ascii(uint16_t keycode);
static void commit_before_ascii(uint16_t keycode);
static bool process_ascii(uint16_t keycode, keyrecord_t *record);

static uint8_t im_state = IM_STATE_PRECOMPOSITION;
static void set_im_state(uint8_t state) {
	im_state = state;
	if(state == IM_STATE_PRECOMPOSITION) {
		reset_cursor();
	}
	im_state_set_user(state);
}
uint8_t get_im_state() { return im_state; }

__attribute__((weak))
void im_state_set_user(uint8_t state) {
	return;
}

static bool _is_practice_mode = false;
bool is_practice_mode() { return _is_practice_mode; }

void reset_ime() {
	im_state = IM_STATE_PRECOMPOSITION;
	_is_practice_mode = false;
	reset_cursor();
	numlock_off();
	im_state_set_user(im_state);
}

static bool is_commit_mode() {
	return (im_state == IM_STATE_HIRAGANA_DIRECT || im_state == IM_STATE_KATAKANA_DIRECT);
}

uint8_t char_count = 0;
// after what count of character
uint8_t cursor_position = 0;
static void reset_cursor() {
	char_count = 0;
	cursor_position = 0;
}
static void handle_cursor(uint16_t keycode) {
	if(is_kana(keycode) || is_ascii_symbol(keycode)
	   || in_range(keycode, KC_A, KC_Z)
	   || in_range(keycode, KC_1, KC_0)) {
		char_count++;
		cursor_position++;
		return;
	}
	switch(keycode) {
	case KC_LEFT:
		if(cursor_position != 0) cursor_position--;
		break;
	case KC_RGHT:
		if(cursor_position < char_count) cursor_position++;
		break;
	case KC_HOME:
		cursor_position = 0;
		break;
	case KC_END:
		cursor_position = char_count;
		break;
	case KC_BSPC:
		if(cursor_position != 0) {
			cursor_position--;
			char_count--;
		}
		break;
	case KC_DEL:
		if(cursor_position < char_count) char_count--;
		break;
	default:
		break;
	}
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

static void capslock_off() {
	if(IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
		register_code(KC_LSFT);
		tap_code(KC_CAPS);
		unregister_code(KC_LSFT);
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
		if(!_is_practice_mode) {
			switch(im_state) {
			case IM_STATE_CONVERT:
			case IM_STATE_COMPOSITION:
			case IM_STATE_PREDICT:
				im_state = IM_STATE_PRECOMPOSITION;
				break;
			default:
				break;
			}
			reset_cursor();
		}
	}
	im_state_set_user(im_state);
}

__attribute__((weak))
uint8_t get_behavior(uint16_t keycode) {
	if(in_range(keycode, KC_A, KC_Z)
	   || in_range(keycode, KC_1, KC_0)) {
		return BEHAVE_HALFWIDTH;
	}
	switch(keycode) {
		case KANA_ZDOT:
		case KANA_ZCOMM:
		case JP_TILD:
		case JP_EXLM:
		case JP_LPRN:
		case JP_RPRN:
		case JP_PLUS:
		case JP_LBRC:
		case JP_RBRC:
		// Yen
		case JP_BSLS:
		case JP_COLN:
		case KC_COMM:
		case KC_DOT:
		case JP_QUES:
			return BEHAVE_COMMIT;
		case JP_GRV:
		case JP_AT:
		case JP_HASH:
		case JP_DLR:
		case JP_PERC:
		case JP_CIRC:
		case JP_ASTR:
		case JP_UNDS:
		case JP_PIPE:
		case KC_SCLN:
		case JP_QUOT:
		case JP_DQT:
		case JP_LT:
		case JP_GT:
		case KC_SLSH:
			return BEHAVE_HALFWIDTH;
		// These often appear in the middle of katakana
		case KC_MINS:
		case KANA_ZSLSH:
		case JP_EQL:
		case JP_AMPR:
			return BEHAVE_COMMIT_ONLY_PRECOMPOSITION;
		case JP_LCBR:
		case JP_RCBR:
			return BEHAVE_COMMIT_BEFORE;
		default:
			return BEHAVE_NORMAL;
	}
}

static bool is_ascii_symbol(uint16_t keycode) {
	if(in_range(keycode, KC_MINS, KC_SLSH)) return true;
	if(in_range(keycode, KC_UNDS, KC_QUES)) return true;
	if(in_range(keycode, KC_EXLM, KC_RPRN)) return true;
	switch(keycode) {
		case JP_RBRC:
		case JP_RCBR:
		case JP_YEN:
		case JP_PIPE:
		case JP_BSLS:
		case JP_UNDS:
			return true;
		default:
			return false;
	}
}
static bool is_shifted_symbol_jp(uint16_t keycode) {
	switch(keycode) {
		case JP_EXLM:
		case JP_DQUO:
		case JP_HASH:
		case JP_DLR:
		case JP_PERC:
		case JP_AMPR:
		case JP_QUOT:
		case JP_LPRN:
		case JP_RPRN:
		case JP_EQL:
		case JP_TILD:
		case JP_PIPE:
		case JP_GRV:
		case JP_LCBR:
		case JP_RCBR:
		case JP_PLUS:
		case JP_ASTR:
		case JP_LABK:
		case JP_RABK:
		case JP_QUES:
		case JP_UNDS:
			return true;
		default:
			return false;
	}
}
static void tap_code_handle_shifted_jp(uint16_t keycode) {
	if(is_shifted_symbol_jp(keycode)) tap_shifted_code(keycode);
	else tap_code(keycode);
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

// PreComposition: char, commit
// Convert: cansel, char, commit
// Predict: (commit by ime), char, commit
static void commit_ascii(uint16_t keycode) {
	if(im_state == IM_STATE_CONVERT) {
		tap_code(KC_ESC);
	}
	tap_code_handle_shifted_jp(keycode);
	if(keyboard_report->mods & MOD_MASK_SHIFT) {
		tap_code_unmods(KC_ENT);
	}
	else tap_code(KC_ENT);
	set_im_state(IM_STATE_PRECOMPOSITION);
}
// to handle 『』 with the same process
// PreComposition: char, convert
// Convert: cansel, commit, char, convert
// Predict: (commit by ime), char, convert
static void commit_before_ascii(uint16_t keycode) {
	if(im_state == IM_STATE_CONVERT) {
		tap_code(KC_ESC);
		tap_code(KC_ENT);
	}
	else if(im_state == IM_STATE_COMPOSITION) {
		tap_code(KC_ENT);
	}
	reset_cursor();
	tap_code_handle_shifted_jp(keycode);
	convert_sequence();
	set_im_state(IM_STATE_CONVERT);
	handle_cursor(keycode);
}
// return value
// true: continue processing
// false: The process is completed by this function
static bool process_ascii(uint16_t keycode, keyrecord_t *record) {
	keyevent_t event = record->event;
	if(!event.pressed) return false;
	if(is_commit_mode()) {
		tap_code_handle_shifted_jp(keycode);
		tap_code(KC_ENT);
		return false;
	}
	switch(get_behavior(keycode)) {
	case BEHAVE_COMMIT:
	// PreComposition: char, commit
	// Convert: cansel, char, commit
	// Predict: (commit by ime), char, commit
		commit_ascii(keycode);
		return false;
	case BEHAVE_HALFWIDTH:
	// PreComposition: char, halfwidth, commit
	// Convert: cansel, char, halfwidth, commit
	// Predict: (commit by ime), char, halfwidth, commit
		if(im_state == IM_STATE_CONVERT) {
			tap_code(KC_ESC);
			tap_code(KC_ENT);
		}
		else if(im_state == IM_STATE_COMPOSITION) {
			tap_code(KC_ENT);
		}
		tap_code_handle_shifted_jp(keycode);
		tap_code(KC_F10);
		tap_code(KC_ENT);
		set_im_state(IM_STATE_PRECOMPOSITION);
		return false;
	case BEHAVE_COMMIT_ONLY_PRECOMPOSITION:
	// PreComposition: char, commit
	// Convert: cansel, char, convert
	// Predict: (commit by ime), char, convert
		if(im_state == IM_STATE_PRECOMPOSITION) {
			tap_code_handle_shifted_jp(keycode);
			tap_code(KC_ENT);
			reset_cursor();
			return false;
		}
		break;
	case BEHAVE_COMMIT_BEFORE:
	// PreComposition: char, convert
	// Convert: cansel, commit, char, convert
	// Predict: (commit by ime), char, convert
		commit_before_ascii(keycode);
		return false;
	default:
		break;
	}
	// PreComposition: char, convert
	// Convert: cansel, char, convert
	// Predict: commit(by ime), char, convert
	if(im_state == IM_STATE_CONVERT) {
		tap_code(KC_ESC);
	}
	else if(im_state == IM_STATE_PREDICT) {
		reset_cursor();
	}
	tap_code_handle_shifted_jp(keycode);
	convert_sequence();
	set_im_state(IM_STATE_CONVERT);
	handle_cursor(keycode);
	return false;
}

bool process_capital_letter(uint16_t keycode, keyrecord_t *record) {
	keyevent_t event = record->event;
	if(!event.pressed) return false;
	uint16_t keycode_base = keymap_key_to_keycode(0, event.key);
	// continue other than shift
	if(!is_shifting_but_other_mod()) {
		tap_code(keycode_base);
		return false;
	}
	// uppercase character
	if(in_range(keycode_base, KC_A, KC_Z)) {
		if(!record->event.pressed) return false;
		commit_ascii(keycode_base);
		return false;
	}
	return true;
}

// return value
// true: continue processing
// false: The process is completed by this function
bool process_kana(uint16_t keycode, keyrecord_t *record) {
	if(!is_kana(keycode)) return true;
	keyevent_t event = record->event;
	if(!event.pressed) return false;
	if(keyboard_report->mods || IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
		return process_capital_letter(keycode, record);
	}
	if(_is_practice_mode) {
		register_kana(keycode);
		return false;
	}
	if(is_commit_mode()) {
		register_kana(keycode);
		tap_code(KC_ENT);
		return false;
	}
	switch(get_behavior(keycode)) {
	case BEHAVE_COMMIT:
	// for punctuation character
	// PreComposition: char, commit
	// Convert: cansel, char, commit
	// Predict: (commit by ime), char, commit
		if(im_state == IM_STATE_CONVERT) {
			tap_code(KC_ESC);
		}
		register_kana(keycode);
		tap_code(KC_ENT);
		set_im_state(IM_STATE_PRECOMPOSITION);
		return false;
	default:
	// for normal character
	// PreComposition: char, convert
	// Convert: cansel, char, convert
	// Predict: commit(by ime), char, convert
		if(im_state == IM_STATE_CONVERT) {
			tap_code(KC_ESC);
		}
		else if(im_state == IM_STATE_PREDICT) {
			reset_cursor();
		}
		register_kana(keycode);
		convert_sequence();
		set_im_state(IM_STATE_CONVERT);
		handle_cursor(keycode);
		return false;
	}
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
				im_state_set_user(im_state);
			}
			// IME: on to off, practice: off
			else {
				// cansel convert
				if(im_state == IM_STATE_CONVERT) {
					tap_code(KC_ESC);
				}
				// TODO: use raw_hid to detect ime state
				numlock_off();
				reset_cursor();
			}
		}
		// IME: off to on, practice: off
		else {
			// TODO: use raw_hid to detect ime state
			numlock_on();
			capslock_off();
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
				im_state_set_user(im_state);
				reset_cursor();
			}
		}
		// IME: off, practice: off to on
		else {
			_is_practice_mode = true;
			numlock_on();
			capslock_off();
		}
		return false;
	}
	if(!is_default_layer_kana()) return true;
	if(is_kana_chord(keycode)) {
		process_kana_chord(keycode, record);
		return false;
	}
	if(is_kana(keycode)) {
		process_kana(keycode, record);
		return false;
	}
	// nothing to do in practice mode after this line
	if(_is_practice_mode) return true;
	if(in_range(keycode, KC_1, KC_0)) {
	// Predict: select candidate, commit
	// Convert: select candidate, commit
	// PreComposition etc: num char, commit
		if(!event.pressed) return false;
		// leave a way to select candidate segment by segment
		if(is_shifting_but_other_mod()) {
			switch(im_state) {
			case IM_STATE_CONVERT:
				tap_code_unmods(keycode);
				return false;
			default:
				break;
			}
		}
		else if(keyboard_report->mods || IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
			return true;
		}
		switch(im_state) {
		case IM_STATE_PREDICT:
		case IM_STATE_CONVERT:
		case IM_STATE_COMPOSITION:
			tap_code(keycode);
			tap_code(KC_ENT);
			set_im_state(IM_STATE_PRECOMPOSITION);
			break;
		default:
			tap_code(keycode);
			tap_code(KC_ENT);
			reset_cursor();
			break;
		}
		return false;
	}
	// put here if mods handling is needed for the keycode
	switch(keycode) {
	case IM_HIRAGANA:
	// PreComposition: turn on hiragana mode
	// HiraganaDirect: turn off hiragana mode
	// KatakanaDirect: turn on hiragana mode
	// Predict: hiragana, commit
	// Convert: hiragana, commit
		if(!event.pressed) return false;
		// leave a way to select candidate segment by segment
		if(is_shifting_but_other_mod()) {
			switch(im_state) {
			case IM_STATE_COMPOSITION:
			case IM_STATE_CONVERT:
				tap_code_unmods(KC_F6);
				return false;
			default:
				return process_capital_letter(keycode, record);
			}
		}
		else if(keyboard_report->mods || IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
			return true;
		}
		switch(im_state) {
		case IM_STATE_PRECOMPOSITION:
		case IM_STATE_KATAKANA_DIRECT:
			tap_code(KC_KANA);
			set_im_state(IM_STATE_HIRAGANA_DIRECT);
			break;
		case IM_STATE_HIRAGANA_DIRECT:
			set_im_state(IM_STATE_PRECOMPOSITION);
			break;
		case IM_STATE_COMPOSITION:
		case IM_STATE_CONVERT:
			tap_code(KC_F6);
			tap_code(KC_ENT);
			set_im_state(IM_STATE_PRECOMPOSITION);
			reset_cursor();
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
		// leave a way to select candidate segment by segment
		if(is_shifting_but_other_mod()) {
			switch(im_state) {
			case IM_STATE_COMPOSITION:
			case IM_STATE_CONVERT:
				tap_code_unmods(KC_F7);
				return false;
			default:
				return process_capital_letter(keycode, record);
			}
		}
		else if(keyboard_report->mods || IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
			return true;
		}
		switch(im_state) {
		case IM_STATE_PRECOMPOSITION:
		case IM_STATE_HIRAGANA_DIRECT:
			tap_shifted_code(KC_KANA);
			set_im_state(IM_STATE_KATAKANA_DIRECT);
			break;
		case IM_STATE_KATAKANA_DIRECT:
			tap_code(KC_KANA);
			set_im_state(IM_STATE_PRECOMPOSITION);
			break;
		case IM_STATE_COMPOSITION:
		case IM_STATE_CONVERT:
			tap_code(KC_F7);
			tap_code(KC_ENT);
			set_im_state(IM_STATE_PRECOMPOSITION);
			reset_cursor();
			break;
		default:
			break;
		}
		return false;
	default:
		break;
	}
	if(keyboard_report->mods || IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
		return process_capital_letter(keycode, record);
	}
	// I want often used parentheses like （）, 「」to be committed after input.
	// I also want to keep a pair of parentheses for conversion.
	// But 『』 don't have enough conversion candidates.
	if(keycode == JP_LCBR || keycode == JP_RCBR) {
	// PreComposition: char, convert
	// Convert: cansel, commit, char, convert
	// Predict: (commit by ime), char, convert
		keycode = (keycode == JP_LCBR) ? JP_LBRC : JP_RBRC;
		if(!event.pressed) return false;
		if(is_commit_mode()) {
			tap_code(keycode);
			tap_code(KC_ENT);
			return false;
		}
		commit_before_ascii(keycode);
		return false;
	}
	if(is_ascii_symbol(keycode)) {
		if(is_commit_mode()) {
			tap_code(keycode);
			tap_code(KC_ENT);
			return false;
		}
		process_ascii(keycode, record);
		return false;
	}
	switch(keycode) {
	case KC_SPC:
	// PreComposition etc: space
	// Composition: convert
	// Predict, Convert: commit
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_COMPOSITION:
			convert_sequence();
			set_im_state(IM_STATE_CONVERT);
		case IM_STATE_CONVERT:
		case IM_STATE_PREDICT:
			tap_code(KC_ENT);
			set_im_state(IM_STATE_PRECOMPOSITION);
			break;
		default:
			tap_code(KC_SPC);
			break;
		}
		return false;
	case KC_ENT:
	// PreComposition etc: return
	// Composition: commit
	// Predict, Convert: hiragana, commit
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_COMPOSITION:
			tap_code(KC_ENT);
			set_im_state(IM_STATE_PRECOMPOSITION);
			break;
		case IM_STATE_CONVERT:
		case IM_STATE_PREDICT:
			tap_code(KC_ESC);
			tap_code(KC_ENT);
			set_im_state(IM_STATE_PRECOMPOSITION);
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
			set_im_state(IM_STATE_PREDICT);
			break;
		case IM_STATE_CONVERT:
			tap_code(KC_ESC);
			predict_sequence();
			set_im_state(IM_STATE_PREDICT);
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
			set_im_state(IM_STATE_PRECOMPOSITION);
			break;
		case IM_STATE_CONVERT:
			tap_code(KC_ESC);
			tap_code(KC_ESC);
			set_im_state(IM_STATE_PRECOMPOSITION);
			break;
		case IM_STATE_PREDICT:
			tap_code(KC_ESC);
			set_im_state(IM_STATE_COMPOSITION);
			break;
		default:
			tap_code(KC_ESC);
			break;
		}
		return false;
	case KC_BSPC:
	case KC_DEL:
	// Predict: cansel, delete char, predict
		if(event.pressed) {
			switch(im_state) {
			case IM_STATE_CONVERT:
				tap_code(KC_ESC);
				tap_code(keycode);
				handle_cursor(keycode);
				// shrink and expand segment don't change cursor on Mozc
				// handle_cursor(keycode);
				if(char_count != 0) {
					convert_sequence();
				}
				else set_im_state(IM_STATE_PRECOMPOSITION);
				break;
			case IM_STATE_PREDICT:
				tap_code(KC_ESC);
				tap_code(keycode);
				handle_cursor(keycode);
				// handle_cursor(keycode);
				if(char_count != 0) {
					predict_sequence();
				}
				else set_im_state(IM_STATE_PRECOMPOSITION);
				break;
			case IM_STATE_COMPOSITION:
				tap_code(keycode);
				handle_cursor(keycode);
				break;
			default:
				register_code(keycode);
				break;
			}
		}
		else { unregister_code(keycode); }
		return false;
	case KC_HOME:
	case KC_END:
	//  : move cursor
	// Convert: move segment
		if(!event.pressed) return false;
		tap_code(keycode);
		if(im_state == IM_STATE_COMPOSITION) {
			handle_cursor(keycode);
		}
		return false;
	case KC_LEFT:
	case KC_RGHT:
	// Composition: move cursor
	// Convert: select next/prev segment
		if(event.pressed) {
			switch(im_state) {
			case IM_STATE_COMPOSITION:
				tap_code(keycode);
				handle_cursor(keycode);
				break;
			case IM_STATE_CONVERT:
				tap_code(keycode);
				convert_sequence();
				break;
			case IM_STATE_PREDICT:
				tap_code(keycode);
			default:
				register_code(keycode);
				break;
			}
		}
		else { unregister_code(keycode); }
		return false;
	case KC_UP:
	// Convert: expand segment
	// Composition: convert backward
		if(event.pressed) {
			switch(im_state) {
			case IM_STATE_COMPOSITION:
				tap_code(KC_UP);
				set_im_state(IM_STATE_CONVERT);
				break;
			case IM_STATE_CONVERT:
				tap_shifted_code(KC_RGHT);
				convert_sequence();
				break;
			default:
				register_code(keycode);
				break;
			}
		}
		else { unregister_code(keycode); }
		return false;
	case KC_DOWN:
	// Composition: convert
	// Convert: shrink segment
		if(event.pressed) {
			switch(im_state) {
			case IM_STATE_COMPOSITION:
				tap_code(KC_DOWN);
				set_im_state(IM_STATE_CONVERT);
				break;
			case IM_STATE_CONVERT:
				tap_shifted_code(KC_LEFT);
				convert_sequence();
				break;
			default:
				register_code(keycode);
				break;
			}
		}
		else { unregister_code(keycode); }
		return false;
	case KC_HENK:
	// PreComposition: reconvert
	// Composition: convert
	// Predict: cansel, convert
	// Convert: cansel to Composition
		if(!event.pressed) return false;
		switch(im_state) {
		case IM_STATE_COMPOSITION:
			convert_sequence();
			set_im_state(IM_STATE_CONVERT);
			break;
		case IM_STATE_PREDICT:
			tap_code(KC_ESC);
			convert_sequence();
			set_im_state(IM_STATE_CONVERT);
			break;
		case IM_STATE_CONVERT:
			tap_code(KC_ESC);
			set_im_state(IM_STATE_COMPOSITION);
			break;
		default:
			tap_code(keycode);
			break;
		}
		return false;
	case KC_CAPS:
		if(!event.pressed) return false;
		// cansel convert
		if(im_state == IM_STATE_CONVERT) {
			tap_code(KC_ESC);
		}
		numlock_off();
		reset_cursor();
		tap_code(JP_ZHTG);
		register_code(KC_LSFT);
		tap_code(KC_CAPS);
		unregister_code(KC_LSFT);
		return false;
	}
	return true;
}
