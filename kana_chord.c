#include "kana_chord.h"
#include "util_user.h" // in_range

#include "keymap.h" // keymap_key_to_keycode
#include "action_layer.h" // default_layer_state, layer_clear
#include "timer.h" // timer_elapsed
#include "nodebug.h"
#include <stddef.h>

static keyevent_t held_as_layer;
static bool is_tapping = false;
static keyevent_t waiting;
static bool await_shift_delay = false;

static uint16_t get_kana_from_keycode(uint16_t keycode);
static layer_state_t get_kana_layer_from_keycode(uint16_t keycode);
static void process_kana_chord(uint16_t keycode, keyrecord_t *record);
static bool is_kana_chord(uint16_t keycode);

bool process_kana(uint16_t keycode, keyrecord_t *record) {
	keyevent_t event = record->event;
	if(biton32(default_layer_state) == L_KANA) {
		if(is_kana_chord(keycode)) {
			process_kana_chord(keycode, record);
			return false;
		}
		if(biton32(default_layer_state | layer_state) == L_KANA) {
			if(is_kana(keycode)) {
				process_record_kana(keycode, record);
				return false;
			}
		}
		else {
			if(keycode == KANA_ROLL) {
				is_tapping = false;
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
			else if(is_kana(keycode) || keycode == KC_NO ) {
				if(event.pressed) {
					if(await_shift_delay) {
						await_shift_delay = false;
						process_record(&(keyrecord_t) {
							.event = waiting
						});
						await_shift_delay = true;
						waiting = event;
					}
					else if(is_tapping) {
						await_shift_delay = true;
						is_tapping = false;
						waiting = event;
					}
					else if(is_kana(keycode)) {
						//test
						process_record_kana(keycode, record);
					}
				}
				else {
					if(await_shift_delay) {
						await_shift_delay = false;
						process_record(&(keyrecord_t) {
							.event = waiting
						});
					}
				}
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
		if(is_tapping || await_shift_delay) {
			uint16_t kana = get_kana_from_keycode(keycode);
			tap_kana(kana, event);
			is_tapping = false;
		}
		if(await_shift_delay) {
			await_shift_delay = false;
			process_record(&(keyrecord_t) {
				.event = waiting
			});
		}
	}
}

void matrix_scan_kana(void) {
	if(await_shift_delay) {
		if(timer_elapsed(waiting.time) >= SHIFTING_DELAY) {
			await_shift_delay = false;
			process_record(&(keyrecord_t) {
				.event = waiting
			});
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
