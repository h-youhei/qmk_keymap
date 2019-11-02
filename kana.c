// #include "action.h" //tap_code
// #include "action_util.h" //keyboard_report
// #include "keymap.h" //keymap_key_to_keycode
#include "quantum.h"

#include "util_user.h" //in_range
#include "kana.h"

bool is_commit_mode = false;
bool is_practice_mode = false;

static inline void commit(void) {
	#ifdef COMMIT_AT_PUNCTUATION
	if(!is_practice_mode) {
		tap_code(KC_ENT);
	}
	#endif
}

bool is_kana(uint16_t keycode) {
	return in_range(keycode, KANA_A, KANA_ZRBRC);
}

void tap_kana(uint16_t kana, keyevent_t event) {
	process_record_kana(kana, &(keyrecord_t){
		// .tap = (tap_t){},
		.event.key = event.key,
		.event.time = event.time,
		.event.pressed = true
	});
}

bool process_record_kana(uint16_t keycode, keyrecord_t *record) {
	if(!is_kana(keycode)) return true;

	keyevent_t event = record->event;
	if(event.pressed) {
		if(keyboard_report->mods || IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
			tap_code(keymap_key_to_keycode(0, event.key));
		}
		else {
			switch(keycode) {
				case KANA_A:
					SEND_STRING("a");
					break;
				case KANA_I:
					SEND_STRING("i");
					break;
				case KANA_U:
					SEND_STRING("u");
					break;
				case KANA_E:
					SEND_STRING("e");
					break;
				case KANA_O:
					SEND_STRING("o");
					break;
				case KANA_KA:
					SEND_STRING("ka");
					break;
				case KANA_KI:
					SEND_STRING("ki");
					break;
				case KANA_KU:
					SEND_STRING("ku");
					break;
				case KANA_KE:
					SEND_STRING("ke");
					break;
				case KANA_KO:
					SEND_STRING("ko");
					break;
				case KANA_SA:
					SEND_STRING("sa");
					break;
				case KANA_SI:
					SEND_STRING("si");
					break;
				case KANA_SU:
					SEND_STRING("su");
					break;
				case KANA_SE:
					SEND_STRING("se");
					break;
				case KANA_SO:
					SEND_STRING("so");
					break;
				case KANA_TA:
					SEND_STRING("ta");
					break;
				case KANA_TI:
					SEND_STRING("ti");
					break;
				case KANA_TU:
					SEND_STRING("tu");
					break;
				case KANA_TE:
					SEND_STRING("te");
					break;
				case KANA_TO:
					SEND_STRING("to");
					break;
				case KANA_NA:
					SEND_STRING("na");
					break;
				case KANA_NI:
					SEND_STRING("ni");
					break;
				case KANA_NU:
					SEND_STRING("nu");
					break;
				case KANA_NE:
					SEND_STRING("ne");
					break;
				case KANA_NO:
					SEND_STRING("no");
					break;
				case KANA_HA:
					SEND_STRING("ha");
					break;
				case KANA_HI:
					SEND_STRING("hi");
					break;
				case KANA_HU:
					SEND_STRING("hu");
					break;
				case KANA_HE:
					SEND_STRING("he");
					break;
				case KANA_HO:
					SEND_STRING("ho");
					break;
				case KANA_MA:
					SEND_STRING("ma");
					break;
				case KANA_MI:
					SEND_STRING("mi");
					break;
				case KANA_MU:
					SEND_STRING("mu");
					break;
				case KANA_ME:
					SEND_STRING("me");
					break;
				case KANA_MO:
					SEND_STRING("mo");
					break;
				case KANA_YA:
					SEND_STRING("ya");
					break;
				case KANA_YU:
					SEND_STRING("yu");
					break;
				case KANA_YO:
					SEND_STRING("yo");
					break;
				case KANA_RA:
					SEND_STRING("ra");
					break;
				case KANA_RI:
					SEND_STRING("ri");
					break;
				case KANA_RU:
					SEND_STRING("ru");
					break;
				case KANA_RE:
					SEND_STRING("re");
					break;
				case KANA_RO:
					SEND_STRING("ro");
					break;
				case KANA_WA:
					SEND_STRING("wa");
					break;
				case KANA_WO:
					SEND_STRING("wo");
					break;
				case KANA_NN:
					SEND_STRING("nn");
					break;
				case KANA_GA:
					SEND_STRING("ga");
					break;
				case KANA_GI:
					SEND_STRING("gi");
					break;
				case KANA_GU:
					SEND_STRING("gu");
					break;
				case KANA_GE:
					SEND_STRING("ge");
					break;
				case KANA_GO:
					SEND_STRING("go");
					break;
				case KANA_ZA:
					SEND_STRING("za");
					break;
				case KANA_ZI:
					SEND_STRING("zi");
					break;
				case KANA_ZU:
					SEND_STRING("zu");
					break;
				case KANA_ZE:
					SEND_STRING("ze");
					break;
				case KANA_ZO:
					SEND_STRING("zo");
					break;
				case KANA_DA:
					SEND_STRING("da");
					break;
				case KANA_DI:
					SEND_STRING("di");
					break;
				case KANA_DU:
					SEND_STRING("du");
					break;
				case KANA_DE:
					SEND_STRING("de");
					break;
				case KANA_DO:
					SEND_STRING("do");
					break;
				case KANA_BA:
					SEND_STRING("ba");
					break;
				case KANA_BI:
					SEND_STRING("bi");
					break;
				case KANA_BU:
					SEND_STRING("bu");
					break;
				case KANA_BE:
					SEND_STRING("be");
					break;
				case KANA_BO:
					SEND_STRING("bo");
					break;
				case KANA_PA:
					SEND_STRING("pa");
					break;
				case KANA_PI:
					SEND_STRING("pi");
					break;
				case KANA_PU:
					SEND_STRING("pu");
					break;
				case KANA_PE:
					SEND_STRING("pe");
					break;
				case KANA_PO:
					SEND_STRING("po");
					break;
				// 拗音
				case KANA_KYA:
					SEND_STRING("kya");
					break;
				case KANA_KYU:
					SEND_STRING("kyu");
					break;
				case KANA_KYE:
					SEND_STRING("kye");
					break;
				case KANA_KYO:
					SEND_STRING("kyo");
					break;
				case KANA_SYA:
					SEND_STRING("sya");
					break;
				case KANA_SYU:
					SEND_STRING("syu");
					break;
				case KANA_SYE:
					SEND_STRING("sye");
					break;
				case KANA_SYO:
					SEND_STRING("syo");
					break;
				case KANA_TYA:
					SEND_STRING("tya");
					break;
				case KANA_TYU:
					SEND_STRING("tyu");
					break;
				case KANA_TYE:
					SEND_STRING("tye");
					break;
				case KANA_TYO:
					SEND_STRING("tyo");
					break;
				case KANA_NYA:
					SEND_STRING("nya");
					break;
				case KANA_NYU:
					SEND_STRING("nyu");
					break;
				case KANA_NYE:
					SEND_STRING("nye");
					break;
				case KANA_NYO:
					SEND_STRING("nyo");
					break;
				case KANA_HYA:
					SEND_STRING("hya");
					break;
				case KANA_HYU:
					SEND_STRING("hyu");
					break;
				case KANA_HYE:
					SEND_STRING("hye");
					break;
				case KANA_HYO:
					SEND_STRING("hyo");
					break;
				case KANA_MYA:
					SEND_STRING("mya");
					break;
				case KANA_MYU:
					SEND_STRING("myu");
					break;
				case KANA_MYE:
					SEND_STRING("mye");
					break;
				case KANA_MYO:
					SEND_STRING("myo");
					break;
				case KANA_RYA:
					SEND_STRING("rya");
					break;
				case KANA_RYU:
					SEND_STRING("ryu");
					break;
				case KANA_RYE:
					SEND_STRING("rye");
					break;
				case KANA_RYO:
					SEND_STRING("ryo");
					break;
				case KANA_GYA:
					SEND_STRING("gya");
					break;
				case KANA_GYU:
					SEND_STRING("gyu");
					break;
				case KANA_GYE:
					SEND_STRING("gye");
					break;
				case KANA_GYO:
					SEND_STRING("gyo");
					break;
				case KANA_JA:
					SEND_STRING("ja");
					break;
				case KANA_JU:
					SEND_STRING("ju");
					break;
				case KANA_JE:
					SEND_STRING("je");
					break;
				case KANA_JO:
					SEND_STRING("jo");
					break;
				case KANA_DYA:
					SEND_STRING("dya");
					break;
				case KANA_DYU:
					SEND_STRING("dyu");
					break;
				case KANA_DYE:
					SEND_STRING("dye");
					break;
				case KANA_DYO:
					SEND_STRING("dyo");
					break;
				case KANA_BYA:
					SEND_STRING("bya");
					break;
				case KANA_BYU:
					SEND_STRING("byu");
					break;
				case KANA_BYE:
					SEND_STRING("bye");
					break;
				case KANA_BYO:
					SEND_STRING("byo");
					break;
				case KANA_PYA:
					SEND_STRING("pya");
					break;
				case KANA_PYU:
					SEND_STRING("pyu");
					break;
				case KANA_PYE:
					SEND_STRING("pye");
					break;
				case KANA_PYO:
					SEND_STRING("pyo");
					break;
				// 外来音
				case KANA_YE:
					SEND_STRING("ye");
					break;
				case KANA_WHA:
					SEND_STRING("wha");
					break;
				case KANA_WI:
					SEND_STRING("wi");
					break;
				case KANA_WE:
					SEND_STRING("we");
					break;
				case KANA_WHO:
					SEND_STRING("who");
					break;
				case KANA_QA:
					SEND_STRING("qa");
					break;
				case KANA_QI:
					SEND_STRING("qi");
					break;
				case KANA_QE:
					SEND_STRING("qe");
					break;
				case KANA_QO:
					SEND_STRING("qo");
					break;
				case KANA_QWA:
					// This is not in Mozc
					SEND_STRING("quxwa");
					break;
				case KANA_GWA:
					SEND_STRING("gwa");
					break;
				case KANA_GWI:
					SEND_STRING("gwi");
					break;
				case KANA_GWE:
					SEND_STRING("gwe");
					break;
				case KANA_GWO:
					SEND_STRING("gwo");
					break;
				/*
				case KANA_SWI:
					// This is not in Mozc
					// SEND_STRING("swi");
					SEND_STRING("suxi");
					break;
				case KANA_ZWI:
					// This is not in Mozc
					// SEND_STRING("zwi");
					SEND_STRING("zuxi");
					break;
				*/
				case KANA_TSA:
					SEND_STRING("tsa");
					break;
				case KANA_TSI:
					SEND_STRING("tsi");
					break;
				case KANA_TSE:
					SEND_STRING("tse");
					break;
				case KANA_TSO:
					SEND_STRING("tso");
					break;
				case KANA_THI:
					SEND_STRING("thi");
					break;
				case KANA_THA:
					SEND_STRING("tha");
					break;
				case KANA_THU:
					SEND_STRING("thu");
					break;
				case KANA_THO:
					SEND_STRING("tho");
					break;
				case KANA_TWU:
					SEND_STRING("twu");
					break;
				case KANA_DHI:
					SEND_STRING("dhi");
					break;
				case KANA_DHA:
					SEND_STRING("dha");
					break;
				case KANA_DHU:
					SEND_STRING("dhu");
					break;
				case KANA_DHO:
					SEND_STRING("dho");
					break;
				case KANA_DWU:
					SEND_STRING("dwu");
					break;
				case KANA_FA:
					SEND_STRING("fa");
					break;
				case KANA_FI:
					SEND_STRING("fi");
					break;
				case KANA_FE:
					SEND_STRING("fe");
					break;
				case KANA_FO:
					SEND_STRING("fo");
					break;
				case KANA_FYA:
					SEND_STRING("fya");
					break;
				case KANA_FYU:
					SEND_STRING("fyu");
					break;
				case KANA_FYO:
					SEND_STRING("fyo");
					break;
				case KANA_VU:
					SEND_STRING("vu");
					break;
				case KANA_VA:
					SEND_STRING("va");
					break;
				case KANA_VI:
					SEND_STRING("vi");
					break;
				case KANA_VE:
					SEND_STRING("ve");
					break;
				case KANA_VO:
					SEND_STRING("vo");
					break;
				case KANA_VYA:
					SEND_STRING("vya");
					break;
				case KANA_VYU:
					SEND_STRING("vyu");
					break;
				case KANA_VYO:
					SEND_STRING("vyo");
					break;
				// 小書き、古文
				case KANA_XA:
					SEND_STRING("xa");
					break;
				case KANA_XI:
					SEND_STRING("xi");
					break;
				case KANA_XU:
					SEND_STRING("xu");
					break;
				case KANA_XE:
					SEND_STRING("xe");
					break;
				case KANA_XO:
					SEND_STRING("xo");
					break;
				case KANA_XKA:
					SEND_STRING("xka");
					break;
				case KANA_XKE:
					SEND_STRING("xke");
					break;
				case KANA_XTU:
					SEND_STRING("xtu");
					break;
				case KANA_XYA:
					SEND_STRING("xya");
					break;
				case KANA_XYU:
					SEND_STRING("xyu");
					break;
				case KANA_XYO:
					SEND_STRING("xyo");
					break;
				case KANA_XWA:
					SEND_STRING("xwa");
					break;
				case KANA_WYI:
					SEND_STRING("wyi");
					break;
				case KANA_WYE:
					SEND_STRING("wye");
					break;
				// 記号
				case KANA_COMM:
					tap_code(KC_COMM);
					commit();
					return false;
					break;
				case KANA_DOT:
					tap_code(KC_DOT);
					commit();
					return false;
					break;
				case KANA_ZSLSH:
					SEND_STRING("z/");
					commit();
					return false;
					break;
				case KANA_ZDOT:
					SEND_STRING("z.");
					commit();
					return false;
					break;
				case KANA_ZCOMM:
					SEND_STRING("z,");
					commit();
					return false;
					break;
				case KANA_ZH:
					SEND_STRING("zh");
					break;
				case KANA_ZJ:
					SEND_STRING("zj");
					break;
				case KANA_ZK:
					SEND_STRING("zk");
					break;
				case KANA_ZL:
					SEND_STRING("zl");
					break;
				case KANA_ZMINS:
					SEND_STRING("z-");
					break;
				case KANA_ZLBRC:
					SEND_STRING("z[");
					break;
				case KANA_ZRBRC:
					SEND_STRING("z]");
					break;
			}
		}
		if(is_commit_mode) tap_code(KC_ENT);
	}
	return false;
}
