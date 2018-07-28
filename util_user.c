#include "util_user.h"
#include "action.h" //register/unregister_code
#include "action_layer.h" // layer_switch_get_layer
#include "keymap.h" //keymap_key_to_keycode

void tap_code(uint16_t keycode) {
	register_code(keycode);
	unregister_code(keycode);
}

bool in_range(uint16_t keycode, uint16_t min, uint16_t max) {
	return keycode >= min && keycode <= max;
}

uint16_t get_keycode_from_keypos(keypos_t keypos) {
	return keymap_key_to_keycode(layer_switch_get_layer(keypos), keypos);
}

uint8_t mod_for_send(uint8_t mod) {
	//0x10 is right mod flag
	return mod&0x10 ? mod<<4 : mod;
}
