#include "util_user.h"
#include "action_layer.h" // layer_switch_get_layer
#include "action_util.h" //keyboard_report, send_keyboard_report, add/del_mods, add/del_weak_mods
#include "keymap.h" //keymap_key_to_keycode

bool in_range(uint16_t keycode, uint16_t min, uint16_t max) {
	return keycode >= min && keycode <= max;
}

uint16_t get_keycode_from_keypos(keypos_t keypos) {
	return keymap_key_to_keycode(layer_switch_get_layer(keypos), keypos);
}

bool is_shifting_but_other_mod() {
	if(keyboard_report->mods & MOD_MASK_SHIFT) {
		return !(keyboard_report->mods & MOD_MASK_CAG);
	}
	return false;
}

void tap_shifted_code(uint16_t keycode) {
	add_weak_mods(MOD_LSFT);
	send_keyboard_report();
	tap_code(keycode);
	del_weak_mods(MOD_LSFT);
	send_keyboard_report();
}

void tap_code_unmods(uint16_t keycode) {
	uint8_t mods = keyboard_report->mods;
	del_mods(mods);
	send_keyboard_report();
	tap_code(keycode);
	add_mods(mods);
	send_keyboard_report();
}

uint8_t mod_for_send(uint8_t mod) {
	//0x10 is right mod flag
	return mod&0x10 ? mod<<4 : mod;
}
