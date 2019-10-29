#pragma once

#include "quantum_keycodes.h"

enum Layer {
	L_BASE,
	L_SHIFT,
#ifdef ENABLE_STABLE_LAYER
	L_STABLE,
#endif
	L_FN,
	L_KANA,
	KANA_LAYER_RANGE,
};

enum user_code {
	CLEAR = SAFE_RANGE,
	IME,
	COMMIT_MODE,
	PRACTICE_MODE,
	JIS_COLN,
	KANA_ROLL,
	KANA_RANGE,
};
