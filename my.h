#pragma once

#include "quantum_keycodes.h"

enum Layer {
	L_BASE,
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
	KANA_ROLL,
	KANA_RANGE,
};
