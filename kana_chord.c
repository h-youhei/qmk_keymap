#include "kana_chord.h"
#include "util_user.h" // in_range

#include "action_layer.h" // default_layer_state, layer_clear
#include "action_util.h" // keyboard_report
#include "keymap.h" // keymap_key_to_keycode
#include "host.h" // IS_HOST_LED_ON
#include "led.h" // USB_LED_CAPS_LOCK
#include "timer.h" // timer_elapsed
#include "util.h" // bitpop32
#include "nodebug.h"
#include <stddef.h>

static uint32_t kana_chord_bit = 0;
static void add_kana_chord(uint32_t kana_note);
static void del_kana_chord(uint32_t kana_note);

static uint16_t tapping;

static uint16_t get_kana_from_keycode(uint16_t keycode);
static uint16_t get_kana_note_from_keycode(uint16_t keycode);
static uint32_t kana_note_to_bit(uint16_t note);
static void process_kana_chord(uint16_t keycode, keyrecord_t *record);
static void register_kana_chord(uint32_t kana_chord);
static bool is_kana_chord(uint16_t keycode);

bool process_kana(uint16_t keycode, keyrecord_t *record) {
	if(biton32(default_layer_state) != L_KANA) return true;
	if(is_kana_chord(keycode)) {
		process_kana_chord(keycode, record);
		return false;
	}
	if(is_kana(keycode)) {
		process_record_kana(keycode, record);
		return false;
	}
	return true;
}

static bool is_recently_pressed = false;
void process_kana_chord(uint16_t keycode, keyrecord_t *record) {
	keyevent_t event = record->event;
	uint32_t note_bit = kana_note_to_bit(get_kana_note_from_keycode(keycode));
	if(event.pressed) {
		if(keyboard_report->mods || IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
			tap_code(keymap_key_to_keycode(0, event.key));
		}
		else {
			is_recently_pressed = true;
			tapping = keycode;
			add_kana_chord(note_bit);
		}
	}
	else {
		if(is_recently_pressed) {
			uint8_t c = bitpop32(kana_chord_bit);
			// tap single key
			if(c == 1) {
				register_kana(get_kana_from_keycode(tapping));
			}
			else if(c > 1) {
				register_kana_chord(kana_chord_bit);
			}
			is_recently_pressed = false;
		}
		del_kana_chord(note_bit);
	}
}

bool is_kana_chord(uint16_t keycode) {
	return in_range(keycode, KANA_CHORD, KANA_CHORD_MAX);
}

void add_kana_chord(uint32_t kana_chord) {
	kana_chord_bit |= kana_chord;
}
void del_kana_chord(uint32_t kana_chord) {
	kana_chord_bit &= ~kana_chord;
}

uint16_t get_kana_from_keycode(uint16_t keycode) {
	return (keycode & 0xFF) + KANA_RANGE;
}

uint16_t get_kana_note_from_keycode(uint16_t keycode) {
	return ((keycode ^ KANA_CHORD) >> 8) & 0x1F;
}

uint32_t kana_note_to_bit(uint16_t chord) {
	// out of bounds
	if (chord > 31) return 0;
	return (1UL << chord);
}

__attribute__((weak))
bool additioral_kana_chord(uint32_t kana_chord) {
	return true;
}

void register_kana_chord(uint32_t kana_chord) {
	if(!additioral_kana_chord(kana_chord)) return;
	if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_A)) {
		register_kana(KANA_XA);
	}
	else if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_I)) {
		register_kana(KANA_XI);
	}
	else if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_U)) {
		register_kana(KANA_XU);
	}
	else if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_E)) {
		register_kana(KANA_XE);
	}
	else if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_O)) {
		register_kana(KANA_XO);
	}
	else if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_YA)) {
		register_kana(KANA_XYA);
	}
	else if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_YU)) {
		register_kana(KANA_XYU);
	}
	else if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_YO)) {
		register_kana(KANA_XYO);
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_A)) {
		register_kana(KANA_A);
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_I)) {
		register_kana(KANA_I);
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_U)) {
		register_kana(KANA_U);
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_E)) {
		register_kana(KANA_E);
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_O)) {
		register_kana(KANA_O);
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_YA)) {
		register_kana(KANA_YA);
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_YU)) {
		register_kana(KANA_YU);
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_YO)) {
		register_kana(KANA_YO);
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_YE)) {
		register_kana(KANA_YE);
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_A)) {
		register_kana(KANA_KA);
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_I)) {
		register_kana(KANA_KI);
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_U)) {
		register_kana(KANA_KU);
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_E)) {
		register_kana(KANA_KE);
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_O)) {
		register_kana(KANA_KO);
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_YA)) {
		register_kana(KANA_KYA);
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_YU)) {
		register_kana(KANA_KYU);
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_YO)) {
		register_kana(KANA_KYO);
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_YE)) {
		register_kana(KANA_KYE);
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_A)) {
		register_kana(KANA_SA);
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_I)) {
		register_kana(KANA_SI);
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_U)) {
		register_kana(KANA_SU);
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_E)) {
		register_kana(KANA_SE);
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_O)) {
		register_kana(KANA_SO);
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_YA)) {
		register_kana(KANA_SYA);
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_YU)) {
		register_kana(KANA_SYU);
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_YO)) {
		register_kana(KANA_SYO);
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_YE)) {
		register_kana(KANA_SYE);
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_A)) {
		register_kana(KANA_TA);
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_I)) {
		register_kana(KANA_TI);
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_U)) {
		register_kana(KANA_TU);
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_E)) {
		register_kana(KANA_TE);
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_O)) {
		register_kana(KANA_TO);
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_YA)) {
		register_kana(KANA_TYA);
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_YU)) {
		register_kana(KANA_TYU);
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_YO)) {
		register_kana(KANA_TYO);
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_YE)) {
		register_kana(KANA_TYE);
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_A)) {
		register_kana(KANA_NA);
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_I)) {
		register_kana(KANA_NI);
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_U)) {
		register_kana(KANA_NU);
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_E)) {
		register_kana(KANA_NE);
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_O)) {
		register_kana(KANA_NO);
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_YA)) {
		register_kana(KANA_NYA);
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_YU)) {
		register_kana(KANA_NYU);
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_YO)) {
		register_kana(KANA_NYO);
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_YE)) {
		register_kana(KANA_NYE);
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_A)) {
		register_kana(KANA_HA);
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_I)) {
		register_kana(KANA_HI);
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_U)) {
		register_kana(KANA_HU);
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_E)) {
		register_kana(KANA_HE);
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_O)) {
		register_kana(KANA_HO);
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_YA)) {
		register_kana(KANA_HYA);
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_YU)) {
		register_kana(KANA_HYU);
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_YO)) {
		register_kana(KANA_HYO);
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_YE)) {
		register_kana(KANA_HYE);
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_A)) {
		register_kana(KANA_MA);
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_I)) {
		register_kana(KANA_MI);
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_U)) {
		register_kana(KANA_MU);
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_E)) {
		register_kana(KANA_ME);
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_O)) {
		register_kana(KANA_MO);
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_YA)) {
		register_kana(KANA_MYA);
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_YU)) {
		register_kana(KANA_MYU);
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_YO)) {
		register_kana(KANA_MYO);
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_YE)) {
		register_kana(KANA_MYE);
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_A)) {
		register_kana(KANA_RA);
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_I)) {
		register_kana(KANA_RI);
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_U)) {
		register_kana(KANA_RU);
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_E)) {
		register_kana(KANA_RE);
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_O)) {
		register_kana(KANA_RO);
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_YA)) {
		register_kana(KANA_RYA);
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_YU)) {
		register_kana(KANA_RYU);
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_YO)) {
		register_kana(KANA_RYO);
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_YE)) {
		register_kana(KANA_RYE);
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_A)) {
		register_kana(KANA_GA);
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_I)) {
		register_kana(KANA_GI);
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_U)) {
		register_kana(KANA_GU);
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_E)) {
		register_kana(KANA_GE);
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_O)) {
		register_kana(KANA_GO);
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_YA)) {
		register_kana(KANA_GYA);
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_YU)) {
		register_kana(KANA_GYU);
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_YO)) {
		register_kana(KANA_GYO);
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_YE)) {
		register_kana(KANA_GYE);
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_A)) {
		register_kana(KANA_ZA);
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_I)) {
		register_kana(KANA_ZI);
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_U)) {
		register_kana(KANA_ZU);
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_E)) {
		register_kana(KANA_ZE);
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_O)) {
		register_kana(KANA_ZO);
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_YA)) {
		register_kana(KANA_ZYA);
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_YU)) {
		register_kana(KANA_ZYU);
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_YO)) {
		register_kana(KANA_ZYO);
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_YE)) {
		register_kana(KANA_ZYE);
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_A)) {
		register_kana(KANA_DA);
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_I)) {
		register_kana(KANA_DI);
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_U)) {
		register_kana(KANA_DU);
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_E)) {
		register_kana(KANA_DE);
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_O)) {
		register_kana(KANA_DO);
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_YA)) {
		register_kana(KANA_DYA);
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_YU)) {
		register_kana(KANA_DYU);
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_YO)) {
		register_kana(KANA_DYO);
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_YE)) {
		register_kana(KANA_DYE);
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_A)) {
		register_kana(KANA_BA);
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_I)) {
		register_kana(KANA_BI);
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_U)) {
		register_kana(KANA_BU);
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_E)) {
		register_kana(KANA_BE);
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_O)) {
		register_kana(KANA_BO);
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_YA)) {
		register_kana(KANA_BYA);
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_YU)) {
		register_kana(KANA_BYU);
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_YO)) {
		register_kana(KANA_BYO);
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_YE)) {
		register_kana(KANA_BYE);
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_A)) {
		register_kana(KANA_PA);
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_I)) {
		register_kana(KANA_PI);
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_U)) {
		register_kana(KANA_PU);
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_E)) {
		register_kana(KANA_PE);
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_O)) {
		register_kana(KANA_PO);
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_YA)) {
		register_kana(KANA_PYA);
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_YU)) {
		register_kana(KANA_PYU);
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_YO)) {
		register_kana(KANA_PYO);
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_YE)) {
		register_kana(KANA_PYE);
	}
	else if(kana_chord == BIT_VOWEL_WA) {
		register_kana(KANA_WA);
	}
	else if(kana_chord == BIT_VOWEL_WO) {
		register_kana(KANA_WO);
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_WA)) {
		register_kana(KANA_WHA);
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_WO)) {
		register_kana(KANA_WHO);
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_WI)) {
		register_kana(KANA_WI);
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_WE)) {
		register_kana(KANA_WE);
	}
	else if(kana_chord == BIT_VOWEL_WI) {
		register_kana(KANA_WYI);
	}
	else if(kana_chord == BIT_VOWEL_WE) {
		register_kana(KANA_WYE);
	}
	else if(kana_chord == BIT_VOWEL_WY) {
		register_kana(KANA_XWA);
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_WA)) {
		register_kana(KANA_QA);
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_WI)) {
		register_kana(KANA_QI);
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_WE)) {
		register_kana(KANA_QE);
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_WO)) {
		register_kana(KANA_QO);
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_WY)) {
		register_kana(KANA_QWA);
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_WA)) {
		register_kana(KANA_GWA);
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_WI)) {
		register_kana(KANA_GWI);
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_WE)) {
		register_kana(KANA_GWE);
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_WO)) {
		register_kana(KANA_GWO);
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_WI)) {
		register_kana(KANA_SWI);
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_WI)) {
		register_kana(KANA_ZWI);
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_WA)) {
		register_kana(KANA_TSA);
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_WI)) {
		register_kana(KANA_TSI);
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_WE)) {
		register_kana(KANA_TSE);
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_WO)) {
		register_kana(KANA_TSO);
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_WA)) {
		register_kana(KANA_FA);
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_WI)) {
		register_kana(KANA_FI);
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_WE)) {
		register_kana(KANA_FE);
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_WO)) {
		register_kana(KANA_FO);
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_WY)) {
		register_kana(KANA_FYU);
	}
	else if(kana_chord == (BIT_CONSONANT_V | BIT_VOWEL_WA)) {
		register_kana(KANA_VA);
	}
	else if(kana_chord == (BIT_CONSONANT_V | BIT_VOWEL_WI)) {
		register_kana(KANA_VI);
	}
	else if(kana_chord == (BIT_CONSONANT_V | BIT_VOWEL_WE)) {
		register_kana(KANA_VE);
	}
	else if(kana_chord == (BIT_CONSONANT_V | BIT_VOWEL_WO)) {
		register_kana(KANA_VO);
	}
	else if(kana_chord == (BIT_CONSONANT_V | BIT_VOWEL_WU)) {
		register_kana(KANA_VU);
	}
	else if(kana_chord == (BIT_CONSONANT_V | BIT_VOWEL_WY)) {
		register_kana(KANA_VYU);
	}
	else if(kana_chord == (BIT_CONSONANT_TH | BIT_VOWEL_WI)) {
		register_kana(KANA_THI);
	}
	else if(kana_chord == (BIT_CONSONANT_TH | BIT_VOWEL_WU)) {
		register_kana(KANA_TWU);
	}
	else if(kana_chord == (BIT_CONSONANT_TH | BIT_VOWEL_WY)) {
		register_kana(KANA_THU);
	}
	else if(kana_chord == (BIT_CONSONANT_DH | BIT_VOWEL_WI)) {
		register_kana(KANA_DHI);
	}
	else if(kana_chord == (BIT_CONSONANT_DH | BIT_VOWEL_WU)) {
		register_kana(KANA_DWU);
	}
	else if(kana_chord == (BIT_CONSONANT_DH | BIT_VOWEL_WY)) {
		register_kana(KANA_DHU);
	}
}
