#ifndef USER_SIMULTANEOUS_H
#define USER_SIMULTANEOUS_H

#include <stdbool.h>
#include <stdint.h>
#include "action.h"
#include "keyboard.h"
#include "quantum_keycodes.h"

enum simultaneous_keycodes {
#if defined(UNICODE_ENABLE)
#error "Cannot enable UNICODE_ENABLE for simultaneous"
#endif
	USER_LAYER_SIMULTANEOUS = 0x9000,
	USER_LAYER_SIMULTANEOUS_MAX = 0x9FFF,
	USER_MOD_SIMULTANEOUS = 0xA000,
	USER_MOD_SIMULTANEOUS_MAX = 0xBFFF,
};

// M-od, S-imultaneous
#define MS(mod, kc) (kc | USER_MOD_SIMULTANEOUS | (((mod) & 0x1F) << 8))

#define SFT_S(kc) MS(MOD_LSFT, kc)
#define LSFT_S(kc) MS(MOD_LSFT, kc)
#define RSFT_S(kc) MS(MOD_RSFT, kc)
#define CTL_S(kc) MS(MOD_LCTL, kc)
#define LCTL_S(kc) MS(MOD_LCTL, kc)
#define RCTL_S(kc) MS(MOD_RCTL, kc)
#define ALT_S(kc) MS(MOD_LALT, kc)
#define LALT_S(kc) MS(MOD_LALT, kc)
#define RALT_S(kc) MS(MOD_RALT, kc)
#define GUI_S(kc) MS(MOD_LGUI, kc)
#define LGUI_S(kc) MS(MOD_LGUI, kc)
#define RGUI_S(kc) MS(MOD_RGUI, kc)

// L-ayer, S-imultaneous
#define LS(layer, kc) (kc | USER_LAYER_SIMULTANEOUS | ((layer & 0xF) << 8))

/*
  Key repeat starts after SIMULTANEOUS_WAIT_TERM.
  It is also used to determine if the process is simultaneous;
  by checking differnce of each key's pressed time and released time are within the term.
*/
#ifndef SIMULTANEOUS_WAIT_TERM
#define SIMULTANEOUS_WAIT_TERM 70
#endif

/*
  SIMULTANEOUSING_TERM is used to determine if the process is simultaneous
  by checking overlapping time the keys are held.
*/
#ifndef SIMULTANEOUSING_TERM
#define SIMULTANEOUSING_TERM 30
#endif

/*
  is_simultaneous_key() is used
  to determine if pressed key takes process_simultaneous or normal process.
  Normal process doesn't have pressing delay.
  For example,
	bool is_simultaneous_key(uint16_t keycode) {
		if(keycode == you want it to take normal process)
			return false
		return IS_KEY(keycode);
	}
*/
bool is_simultaneous_key(uint16_t keycode);

/*
  has_simultaneous_priority_to_a is used
  to determine which key is mod or layer and is tapping key;
  when simultaneous tapping that both keys are simultaneous_mod or simultaneous_layer.
  The one that has priority is treated mod or layer.
  Default behavior:
    shift has priority to others
    if both key is not shift,
    treat first released key as key the other as mod.
*/
bool has_simultaneous_priority_to_a(uint16_t keycode_a, uint16_t keycode_b);

bool is_simultaneous_layer(uint16_t keycode);
bool is_simultaneous_mod(uint16_t keycode);
uint8_t get_simultaneous_mod_from_keycode(uint16_t keycode);
uint8_t get_simultaneous_layer_from_keycode(uint16_t keycode);

/* call this function from process_record_user */
bool process_simultaneous(uint16_t keycode, keyrecord_t *record);
/* call this function from matrix_scan_user */
void matrix_scan_simultaneous(void);

#endif
