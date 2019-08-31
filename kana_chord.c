#include "kana_chord.h"
#include "util_user.h" // in_range

#include "keymap.h" // keymap_key_to_keycode
#include "action_layer.h" // default_layer_state
#include "nodebug.h"
#include <stddef.h>

static keyevent_t held_as_layer;
static bool is_tapping = false;

static uint16_t get_kana_from_keycode(uint16_t keycode);
static layer_state_t get_kana_layer_from_keycode(uint16_t keycode);
static void process_kana_chord(uint16_t keycode, keyrecord_t *record);
static bool is_kana_chord(uint16_t keycode);

bool process_kana(uint16_t keycode, keyrecord_t *record) {
	if(biton32(default_layer_state) == L_KANA) {
		if(is_kana_chord(keycode)) {
			process_kana_chord(keycode, record);
			return false;
		}
		is_tapping = false;
		if(is_kana(keycode)) {
			process_record_kana(keycode, record);
			return false;
		}
		else if(keycode == KANA_ROLL) {
			keyevent_t event = record->event;
			if(event.pressed) {
				// first key
				uint16_t keycode = keymap_key_to_keycode(L_KANA, held_as_layer.key);
				uint16_t kana = get_kana_from_keycode(keycode);
				tap_kana(kana, held_as_layer);
				// second key
				keycode = keymap_key_to_keycode(L_KANA, event.key);
				if(is_kana_chord(keycode)) {
					keycode = get_kana_from_keycode(keycode);
				}
				tap_kana(keycode, event);
			}
			return false;
		}
	}
	return true;
}

void process_kana_chord(uint16_t keycode, keyrecord_t *record) {
	keyevent_t event = record->event;
	uint8_t layer = get_kana_layer_from_keycode(keycode);
	if(event.pressed) {
		held_as_layer = event;
		layer_on(layer);
		is_tapping = true;
	}
	else {
		layer_off(layer);
		if(is_tapping) {
			uint16_t kana = get_kana_from_keycode(keycode);
			tap_kana(kana, event);
		}
	}
}

bool is_kana_chord(uint16_t keycode) {
	return in_range(keycode, KANA_CHORD, KANA_CHORD_MAX);
}

uint16_t get_kana_from_keycode(uint16_t keycode) {
	return (keycode & 0xFF) + KANA_RANGE;
}

layer_state_t get_kana_layer_from_keycode(uint16_t keycode) {
	return ((keycode ^ KANA_CHORD) >> 8) & 0x1F;
}
