#include "kana_chord.h"
#include "util_user.h" // in_range
#include "ime.h" // process_ime

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
static uint16_t get_kana_from_chord(uint32_t kana_chord);

static bool is_recently_pressed = false;
void process_kana_chord(uint16_t keycode, keyrecord_t *record) {
	keyevent_t event = record->event;
	uint32_t note_bit = kana_note_to_bit(get_kana_note_from_keycode(keycode));
	if(event.pressed) {
		if(keyboard_report->mods || IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
			process_capital_letter(keycode, record);
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
				tap_kana(get_kana_from_keycode(tapping), event);
			}
			else if(c > 1) {
				tap_kana(get_kana_from_chord(kana_chord_bit), event);
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
uint16_t additional_kana_chord(uint32_t kana_chord) {
	return KC_NO;
}

uint16_t get_kana_from_chord(uint32_t kana_chord) {
	uint16_t additional_kana = additional_kana_chord(kana_chord);
	if(additional_kana != KC_NO) return additional_kana;

	if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_A)) {
		return KANA_XA;
	}
	else if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_I)) {
		return KANA_XI;
	}
	else if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_U)) {
		return KANA_XU;
	}
	else if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_E)) {
		return KANA_XE;
	}
	else if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_O)) {
		return KANA_XO;
	}
	else if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_YA)) {
		return KANA_XYA;
	}
	else if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_YU)) {
		return KANA_XYU;
	}
	else if(kana_chord == (BIT_CONSONANT_X | BIT_VOWEL_YO)) {
		return KANA_XYO;
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_A)) {
		return KANA_A;
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_I)) {
		return KANA_I;
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_U)) {
		return KANA_U;
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_E)) {
		return KANA_E;
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_O)) {
		return KANA_O;
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_YA)) {
		return KANA_YA;
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_YU)) {
		return KANA_YU;
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_YO)) {
		return KANA_YO;
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_YE)) {
		return KANA_YE;
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_A)) {
		return KANA_KA;
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_I)) {
		return KANA_KI;
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_U)) {
		return KANA_KU;
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_E)) {
		return KANA_KE;
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_O)) {
		return KANA_KO;
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_YA)) {
		return KANA_KYA;
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_YU)) {
		return KANA_KYU;
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_YO)) {
		return KANA_KYO;
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_YE)) {
		return KANA_KYE;
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_A)) {
		return KANA_SA;
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_I)) {
		return KANA_SI;
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_U)) {
		return KANA_SU;
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_E)) {
		return KANA_SE;
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_O)) {
		return KANA_SO;
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_YA)) {
		return KANA_SYA;
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_YU)) {
		return KANA_SYU;
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_YO)) {
		return KANA_SYO;
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_YE)) {
		return KANA_SYE;
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_A)) {
		return KANA_TA;
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_I)) {
		return KANA_TI;
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_U)) {
		return KANA_TU;
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_E)) {
		return KANA_TE;
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_O)) {
		return KANA_TO;
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_YA)) {
		return KANA_TYA;
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_YU)) {
		return KANA_TYU;
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_YO)) {
		return KANA_TYO;
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_YE)) {
		return KANA_TYE;
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_A)) {
		return KANA_NA;
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_I)) {
		return KANA_NI;
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_U)) {
		return KANA_NU;
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_E)) {
		return KANA_NE;
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_O)) {
		return KANA_NO;
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_YA)) {
		return KANA_NYA;
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_YU)) {
		return KANA_NYU;
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_YO)) {
		return KANA_NYO;
	}
	else if(kana_chord == (BIT_CONSONANT_N | BIT_VOWEL_YE)) {
		return KANA_NYE;
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_A)) {
		return KANA_HA;
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_I)) {
		return KANA_HI;
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_U)) {
		return KANA_HU;
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_E)) {
		return KANA_HE;
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_O)) {
		return KANA_HO;
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_YA)) {
		return KANA_HYA;
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_YU)) {
		return KANA_HYU;
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_YO)) {
		return KANA_HYO;
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_YE)) {
		return KANA_HYE;
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_A)) {
		return KANA_MA;
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_I)) {
		return KANA_MI;
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_U)) {
		return KANA_MU;
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_E)) {
		return KANA_ME;
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_O)) {
		return KANA_MO;
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_YA)) {
		return KANA_MYA;
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_YU)) {
		return KANA_MYU;
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_YO)) {
		return KANA_MYO;
	}
	else if(kana_chord == (BIT_CONSONANT_M | BIT_VOWEL_YE)) {
		return KANA_MYE;
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_A)) {
		return KANA_RA;
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_I)) {
		return KANA_RI;
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_U)) {
		return KANA_RU;
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_E)) {
		return KANA_RE;
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_O)) {
		return KANA_RO;
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_YA)) {
		return KANA_RYA;
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_YU)) {
		return KANA_RYU;
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_YO)) {
		return KANA_RYO;
	}
	else if(kana_chord == (BIT_CONSONANT_R | BIT_VOWEL_YE)) {
		return KANA_RYE;
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_A)) {
		return KANA_GA;
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_I)) {
		return KANA_GI;
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_U)) {
		return KANA_GU;
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_E)) {
		return KANA_GE;
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_O)) {
		return KANA_GO;
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_YA)) {
		return KANA_GYA;
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_YU)) {
		return KANA_GYU;
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_YO)) {
		return KANA_GYO;
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_YE)) {
		return KANA_GYE;
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_A)) {
		return KANA_ZA;
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_I)) {
		return KANA_ZI;
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_U)) {
		return KANA_ZU;
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_E)) {
		return KANA_ZE;
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_O)) {
		return KANA_ZO;
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_YA)) {
		return KANA_ZYA;
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_YU)) {
		return KANA_ZYU;
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_YO)) {
		return KANA_ZYO;
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_YE)) {
		return KANA_ZYE;
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_A)) {
		return KANA_DA;
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_I)) {
		return KANA_DI;
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_U)) {
		return KANA_DU;
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_E)) {
		return KANA_DE;
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_O)) {
		return KANA_DO;
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_YA)) {
		return KANA_DYA;
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_YU)) {
		return KANA_DYU;
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_YO)) {
		return KANA_DYO;
	}
	else if(kana_chord == (BIT_CONSONANT_D | BIT_VOWEL_YE)) {
		return KANA_DYE;
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_A)) {
		return KANA_BA;
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_I)) {
		return KANA_BI;
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_U)) {
		return KANA_BU;
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_E)) {
		return KANA_BE;
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_O)) {
		return KANA_BO;
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_YA)) {
		return KANA_BYA;
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_YU)) {
		return KANA_BYU;
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_YO)) {
		return KANA_BYO;
	}
	else if(kana_chord == (BIT_CONSONANT_B | BIT_VOWEL_YE)) {
		return KANA_BYE;
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_A)) {
		return KANA_PA;
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_I)) {
		return KANA_PI;
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_U)) {
		return KANA_PU;
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_E)) {
		return KANA_PE;
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_O)) {
		return KANA_PO;
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_YA)) {
		return KANA_PYA;
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_YU)) {
		return KANA_PYU;
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_YO)) {
		return KANA_PYO;
	}
	else if(kana_chord == (BIT_CONSONANT_P | BIT_VOWEL_YE)) {
		return KANA_PYE;
	}
	else if(kana_chord == BIT_VOWEL_WA) {
		return KANA_WA;
	}
	else if(kana_chord == BIT_VOWEL_WO) {
		return KANA_WO;
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_WA)) {
		return KANA_WHA;
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_WO)) {
		return KANA_WHO;
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_WI)) {
		return KANA_WI;
	}
	else if(kana_chord == (BIT_CONSONANT_A | BIT_VOWEL_WE)) {
		return KANA_WE;
	}
	else if(kana_chord == BIT_VOWEL_WI) {
		return KANA_WYI;
	}
	else if(kana_chord == BIT_VOWEL_WE) {
		return KANA_WYE;
	}
	else if(kana_chord == BIT_VOWEL_WY) {
		return KANA_XWA;
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_WA)) {
		return KANA_QA;
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_WI)) {
		return KANA_QI;
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_WE)) {
		return KANA_QE;
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_WO)) {
		return KANA_QO;
	}
	else if(kana_chord == (BIT_CONSONANT_K | BIT_VOWEL_WY)) {
		return KANA_QWA;
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_WA)) {
		return KANA_GWA;
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_WI)) {
		return KANA_GWI;
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_WE)) {
		return KANA_GWE;
	}
	else if(kana_chord == (BIT_CONSONANT_G | BIT_VOWEL_WO)) {
		return KANA_GWO;
	}
	else if(kana_chord == (BIT_CONSONANT_S | BIT_VOWEL_WI)) {
		return KANA_SWI;
	}
	else if(kana_chord == (BIT_CONSONANT_Z | BIT_VOWEL_WI)) {
		return KANA_ZWI;
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_WA)) {
		return KANA_TSA;
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_WI)) {
		return KANA_TSI;
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_WE)) {
		return KANA_TSE;
	}
	else if(kana_chord == (BIT_CONSONANT_T | BIT_VOWEL_WO)) {
		return KANA_TSO;
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_WA)) {
		return KANA_FA;
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_WI)) {
		return KANA_FI;
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_WE)) {
		return KANA_FE;
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_WO)) {
		return KANA_FO;
	}
	else if(kana_chord == (BIT_CONSONANT_H | BIT_VOWEL_WY)) {
		return KANA_FYU;
	}
	else if(kana_chord == (BIT_CONSONANT_V | BIT_VOWEL_WA)) {
		return KANA_VA;
	}
	else if(kana_chord == (BIT_CONSONANT_V | BIT_VOWEL_WI)) {
		return KANA_VI;
	}
	else if(kana_chord == (BIT_CONSONANT_V | BIT_VOWEL_WE)) {
		return KANA_VE;
	}
	else if(kana_chord == (BIT_CONSONANT_V | BIT_VOWEL_WO)) {
		return KANA_VO;
	}
	else if(kana_chord == (BIT_CONSONANT_V | BIT_VOWEL_WU)) {
		return KANA_VU;
	}
	else if(kana_chord == (BIT_CONSONANT_V | BIT_VOWEL_WY)) {
		return KANA_VYU;
	}
	else if(kana_chord == (BIT_CONSONANT_TH | BIT_VOWEL_WI)) {
		return KANA_THI;
	}
	else if(kana_chord == (BIT_CONSONANT_TH | BIT_VOWEL_WU)) {
		return KANA_TWU;
	}
	else if(kana_chord == (BIT_CONSONANT_TH | BIT_VOWEL_WY)) {
		return KANA_THU;
	}
	else if(kana_chord == (BIT_CONSONANT_DH | BIT_VOWEL_WI)) {
		return KANA_DHI;
	}
	else if(kana_chord == (BIT_CONSONANT_DH | BIT_VOWEL_WU)) {
		return KANA_DWU;
	}
	else if(kana_chord == (BIT_CONSONANT_DH | BIT_VOWEL_WY)) {
		return KANA_DHU;
	}
	return KC_NO;
}
