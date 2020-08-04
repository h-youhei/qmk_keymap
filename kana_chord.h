#ifndef kana_chord_h_INCLUDED
#define kana_chord_h_INCLUDED

#include "my.h"
#include "kana.h"

#if !defined(LAYER_KANA)
#warning "LAYER_KANA is not defined. It is set to default value 1"
#define LAYER_KANA 1
#endif

#if !defined(BIT_NEIGHBOR_VOWEL_A)
#error "BIT_NEIGHBOR_VOWEL_A should be defined"
#endif
#if !defined(BIT_NEIGHBOR_VOWEL_I)
#error "BIT_NEIGHBOR_VOWEL_I should be defined"
#endif
#if !defined(BIT_NEIGHBOR_VOWEL_U)
#error "BIT_NEIGHBOR_VOWEL_U should be defined"
#endif
#if !defined(BIT_NEIGHBOR_VOWEL_E)
#error "BIT_NEIGHBOR_VOWEL_E should be defined"
#endif
#if !defined(BIT_NEIGHBOR_VOWEL_O)
#error "BIT_NEIGHBOR_VOWEL_O should be defined"
#endif
#if !defined(BIT_NEIGHBOR_VOWEL_YO)
#error "BIT_NEIGHBOR_VOWEL_YO should be defined"
#endif
#if !defined(BIT_NEIGHBOR_CONSONANT_A)
#error "BIT_NEIGHBOR_CONSONANT_A should be defined"
#endif
#if !defined(BIT_NEIGHBOR_CONSONANT_K)
#error "BIT_NEIGHBOR_CONSONANT_K should be defined"
#endif
#if !defined(BIT_NEIGHBOR_CONSONANT_S)
#error "BIT_NEIGHBOR_CONSONANT_S should be defined"
#endif
#if !defined(BIT_NEIGHBOR_CONSONANT_T)
#error "BIT_NEIGHBOR_CONSONANT_T should be defined"
#endif
#if !defined(BIT_NEIGHBOR_CONSONANT_H)
#error "BIT_NEIGHBOR_CONSONANT_H should be defined"
#endif
#if !defined(BIT_NEIGHBOR_CONSONANT_R)
#error "BIT_NEIGHBOR_CONSONANT_R should be defined"
#endif
#if !defined(BIT_CONSONANT_TH)
#error "BIT_CONSONANT_TH should be defined"
#endif
#if !defined(BIT_NEIGHBOR_CONSONANT_TH)
#error "BIT_NEIGHBOR_CONSONANT_TH should be defined"
#endif
#if !defined(BIT_CONSONANT_V)
#error "BIT_CONSONANT_V should be defined"
#endif

// You can define additional kana_chord by overriding this function.
// Branch by kana_chord and return kana.
// You should return KC_NO other than additional_kana_chord.
uint16_t additioral_kana_chord(uint32_t kana_chord);

bool is_kana_chord(uint16_t keycode);

// return value
// true: continue processing
// false: The process is completed by this function
void process_kana_chord(uint16_t keycode, keyrecord_t *record);

enum kana_notes {
	VOWEL_A,
	VOWEL_I,
	VOWEL_U,
	VOWEL_E,
	VOWEL_O,
	VOWEL_YA,
	VOWEL_YU,
	VOWEL_YO,
	VOWEL_YE,
	CONSONANT_A,
	CONSONANT_K,
	CONSONANT_S,
	CONSONANT_T,
	CONSONANT_N,
	CONSONANT_H,
	CONSONANT_M,
	CONSONANT_R,
	CONSONANT_P,
};

enum kana_chord_keycodes {
#if defined(UNICODE_ENABLE)
#error "Cannot enable both UNICODE_ENABLE and kana input"
#endif
	KANA_CHORD = 0x8000,
	KANA_CHORD_MAX = 0x9FFF
};

#define KANA_CHORD(notes, kc) ( (kc - KANA_RANGE) | KANA_CHORD | ((notes & 0x1F) << 8) )
#define VOWEL_A(kc) KANA_CHORD(VOWEL_A, kc)
#define VOWEL_I(kc) KANA_CHORD(VOWEL_I, kc)
#define VOWEL_U(kc) KANA_CHORD(VOWEL_U, kc)
#define VOWEL_E(kc) KANA_CHORD(VOWEL_E, kc)
#define VOWEL_O(kc) KANA_CHORD(VOWEL_O, kc)
#define VOWEL_YA(kc) KANA_CHORD(VOWEL_YA, kc)
#define VOWEL_YU(kc) KANA_CHORD(VOWEL_YU, kc)
#define VOWEL_YO(kc) KANA_CHORD(VOWEL_YO, kc)
#define VOWEL_YE(kc) KANA_CHORD(VOWEL_YE, kc)
#define CONSONANT_A(kc) KANA_CHORD(CONSONANT_A, kc)
#define CONSONANT_K(kc) KANA_CHORD(CONSONANT_K, kc)
#define CONSONANT_S(kc) KANA_CHORD(CONSONANT_S, kc)
#define CONSONANT_T(kc) KANA_CHORD(CONSONANT_T, kc)
#define CONSONANT_N(kc) KANA_CHORD(CONSONANT_N, kc)
#define CONSONANT_H(kc) KANA_CHORD(CONSONANT_H, kc)
#define CONSONANT_M(kc) KANA_CHORD(CONSONANT_M, kc)
#define CONSONANT_R(kc) KANA_CHORD(CONSONANT_R, kc)
#define CONSONANT_W(kc) KANA_CHORD(CONSONANT_W, kc)
#define CONSONANT_P(kc) KANA_CHORD(CONSONANT_P, kc)

#define BIT_VOWEL_A (1UL << VOWEL_A)
#define BIT_VOWEL_I (1UL << VOWEL_I)
#define BIT_VOWEL_U (1UL << VOWEL_U)
#define BIT_VOWEL_E (1UL << VOWEL_E)
#define BIT_VOWEL_O (1UL << VOWEL_O)
#define BIT_VOWEL_YA (1UL << VOWEL_YA)
#define BIT_VOWEL_YU (1UL << VOWEL_YU)
#define BIT_VOWEL_YO (1UL << VOWEL_YO)
#define BIT_VOWEL_YE (1UL << VOWEL_YE)
#define BIT_VOWEL_WA (BIT_VOWEL_A | BIT_NEIGHBOR_VOWEL_A)
#define BIT_VOWEL_WI (BIT_VOWEL_I | BIT_NEIGHBOR_VOWEL_I)
#define BIT_VOWEL_WU (BIT_VOWEL_U | BIT_NEIGHBOR_VOWEL_U)
#define BIT_VOWEL_WE (BIT_VOWEL_E | BIT_NEIGHBOR_VOWEL_E)
#define BIT_VOWEL_WO (BIT_VOWEL_O | BIT_NEIGHBOR_VOWEL_O)
#define BIT_VOWEL_WY (BIT_VOWEL_YO | BIT_NEIGHBOR_VOWEL_YO)
#define BIT_CONSONANT_A (1UL << CONSONANT_A)
#define BIT_CONSONANT_K (1UL << CONSONANT_K)
#define BIT_CONSONANT_S (1UL << CONSONANT_S)
#define BIT_CONSONANT_T (1UL << CONSONANT_T)
#define BIT_CONSONANT_N (1UL << CONSONANT_N)
#define BIT_CONSONANT_H (1UL << CONSONANT_H)
#define BIT_CONSONANT_M (1UL << CONSONANT_M)
#define BIT_CONSONANT_R (1UL << CONSONANT_R)
#define BIT_CONSONANT_P (1UL << CONSONANT_P)
#define BIT_CONSONANT_X (BIT_CONSONANT_A | BIT_NEIGHBOR_CONSONANT_A)
#define BIT_CONSONANT_G (BIT_CONSONANT_K | BIT_NEIGHBOR_CONSONANT_K)
#define BIT_CONSONANT_Z (BIT_CONSONANT_S | BIT_NEIGHBOR_CONSONANT_S)
#define BIT_CONSONANT_D (BIT_CONSONANT_T | BIT_NEIGHBOR_CONSONANT_T)
#define BIT_CONSONANT_B (BIT_CONSONANT_H | BIT_NEIGHBOR_CONSONANT_H)
#define BIT_CONSONANT_DH (BIT_CONSONANT_TH | BIT_NEIGHBOR_CONSONANT_TH)

#endif // kana_chord_h_INCLUDED
