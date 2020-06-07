#pragma once

#include "quantum_keycodes.h"

enum Layer {
	L_BASE,
	L_SHIFT,
#ifdef ENABLE_STABLE_LAYER
	L_STABLE,
#endif
	L_FN,
#ifndef NO_JAPANESE
	L_KANA,
#endif
};

enum user_code {
	CLEAR = SAFE_RANGE,
	IME,
	PRACTICE_MODE,
#ifndef NO_JAPANESE
	COMMIT_MODE,
	JIS_COLN,
	KANA_ROLL,
	KANA_RANGE,
#endif
};
