#ifndef kana_chord_h_INCLUDED
#define kana_chord_h_INCLUDED

#include "my.h"
#include "kana.h"

bool process_kana(uint16_t keycode, keyrecord_t *record);
void matrix_scan_kana(void);
// used for avoiding shift remaining
#ifndef SHIFTING_DELAY
#define SHIFTING_DELAY 100
#endif

enum kana_layer {
	L_KANA_A = KANA_LAYER_RANGE,
	L_KANA_I,
	L_KANA_U,
	L_KANA_E,
	L_KANA_O,
	L_KANA_YA,
	L_KANA_YU,
	L_KANA_YO,
	L_KANA_YE,
	L_KANA_SIGN,
	L_KANA_PARN,
	L_KANA_K,
	L_KANA_S,
	L_KANA_T,
	L_KANA_N,
	L_KANA_H,
	L_KANA_M,
	L_KANA_R,
	L_KANA_W,
	L_KANA_F,
	L_KANA_Q,
	L_KANA_TH,
};

enum kana_chord_keycodes {
#if defined(UNICODE_ENABLE)
#error "Cannot enable UNICODE_ENABLE for kana input"
#endif
	KANA_CHORD = 0x8000,
	KANA_CHORD_MAX = 0x9FFF
};

#define KANA_T(layer, kc) ( (kc - KANA_RANGE) | KANA_CHORD | ((layer & 0x1F) << 8) )
#define KANA_A_T(kc) KANA_T(L_KANA_A, kc)
#define KANA_I_T(kc) KANA_T(L_KANA_I, kc)
#define KANA_U_T(kc) KANA_T(L_KANA_U, kc)
#define KANA_E_T(kc) KANA_T(L_KANA_E, kc)
#define KANA_O_T(kc) KANA_T(L_KANA_O, kc)
#define KANA_YA_T(kc) KANA_T(L_KANA_YA, kc)
#define KANA_YU_T(kc) KANA_T(L_KANA_YU, kc)
#define KANA_YO_T(kc) KANA_T(L_KANA_YO, kc)
#define KANA_YE_T(kc) KANA_T(L_KANA_YE, kc)
#define KANA_K_T(kc) KANA_T(L_KANA_K, kc)
#define KANA_S_T(kc) KANA_T(L_KANA_S, kc)
#define KANA_T_T(kc) KANA_T(L_KANA_T, kc)
#define KANA_N_T(kc) KANA_T(L_KANA_N, kc)
#define KANA_H_T(kc) KANA_T(L_KANA_H, kc)
#define KANA_M_T(kc) KANA_T(L_KANA_M, kc)
#define KANA_R_T(kc) KANA_T(L_KANA_R, kc)
#define KANA_W_T(kc) KANA_T(L_KANA_W, kc)
#define KANA_F_T(kc) KANA_T(L_KANA_F, kc)
#define KANA_Q_T(kc) KANA_T(L_KANA_Q, kc)
#define KANA_TH_T(kc) KANA_T(L_KANA_TH, kc)

#endif // kana_chord_h_INCLUDED
