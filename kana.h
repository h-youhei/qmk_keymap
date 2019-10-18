#ifndef kana_h_INCLUDED
#define kana_h_INCLUDED

#include "action.h" //keyrecord_t
#include "my.h" //KANA_RANGE

extern bool is_playing_kana_typing_game;
bool is_kana(uint16_t keycode);
void tap_kana(uint16_t kana, keyevent_t event);
bool process_record_kana(uint16_t keycode, keyrecord_t *record);

enum kana_code {
	KANA_A = KANA_RANGE,
	KANA_I,
	KANA_U,
	KANA_E,
	KANA_O,
	KANA_KA,
	KANA_KI,
	KANA_KU,
	KANA_KE,
	KANA_KO,
	KANA_SA,
	KANA_SI,
	KANA_SU,
	KANA_SE,
	KANA_SO,
	KANA_TA,
	KANA_TI,
	KANA_TU,
	KANA_TE,
	KANA_TO,
	KANA_NA,
	KANA_NI,
	KANA_NU,
	KANA_NE,
	KANA_NO,
	KANA_HA,
	KANA_HI,
	KANA_HU,
	KANA_HE,
	KANA_HO,
	KANA_MA,
	KANA_MI,
	KANA_MU,
	KANA_ME,
	KANA_MO,
	KANA_YA,
	KANA_YU,
	KANA_YO,
	KANA_RA,
	KANA_RI,
	KANA_RU,
	KANA_RE,
	KANA_RO,
	KANA_WA,
	KANA_WO,
	KANA_NN,
	KANA_GA,
	KANA_GI,
	KANA_GU,
	KANA_GE,
	KANA_GO,
	KANA_ZA,
	KANA_ZI,
	KANA_ZU,
	KANA_ZE,
	KANA_ZO,
	KANA_DA,
	KANA_DI,
	KANA_DU,
	KANA_DE,
	KANA_DO,
	KANA_BA,
	KANA_BI,
	KANA_BU,
	KANA_BE,
	KANA_BO,
	KANA_PA,
	KANA_PI,
	KANA_PU,
	KANA_PE,
	KANA_PO,
	// 拗音
	KANA_KYA,
	KANA_KYU,
	KANA_KYO,
	KANA_SYA,
	KANA_SYU,
	KANA_SYO,
	KANA_TYA,
	KANA_TYU,
	KANA_TYO,
	KANA_NYA,
	KANA_NYU,
	KANA_NYO,
	KANA_HYA,
	KANA_HYU,
	KANA_HYO,
	KANA_MYA,
	KANA_MYU,
	KANA_MYO,
	KANA_RYA,
	KANA_RYU,
	KANA_RYO,
	KANA_GYA,
	KANA_GYU,
	KANA_GYO,
	KANA_JA,
	KANA_JU,
	KANA_JO,
	KANA_DYA,
	KANA_DYU,
	KANA_DYO,
	KANA_BYA,
	KANA_BYU,
	KANA_BYO,
	KANA_PYA,
	KANA_PYU,
	KANA_PYO,
	// 外来音
	KANA_YE, //イェ
	KANA_WHA, //ウァ
	KANA_WI, //ウィ
	KANA_WE,
	KANA_WHO,
	KANA_QA, //クァ
	KANA_QI,
	KANA_QE,
	KANA_QO,
	KANA_GWA, //グァ
	KANA_GWI,
	KANA_GWE,
	KANA_GWO,
	KANA_SWI, //スィ
	KANA_SYE, //シェ
	KANA_ZWI, //ズィ
	KANA_JE, //ジェ
	KANA_TSA, //ツァ
	KANA_TSI,
	KANA_TSE,
	KANA_TSO,
	KANA_TYE, //チェ
	KANA_THI, //ティ
	KANA_THU, //テュ
	KANA_TWU, //トゥ
	KANA_DHI, //ディ
	KANA_DHU, //デュ
	KANA_DWU, //ドゥ
	KANA_DYE, //ヂェ
	KANA_FA, //ファ
	KANA_FI,
	KANA_FE,
	KANA_FO,
	KANA_FYU, //フュ
	KANA_VU, //ヴ
	KANA_VA, //ヴァ
	KANA_VI,
	KANA_VE,
	KANA_VO,
	KANA_VYU, //ヴュ
	// 小書き、古文
	KANA_XA,
	KANA_XI,
	KANA_XU,
	KANA_XE,
	KANA_XO,
	KANA_XKA,
	KANA_XKE,
	KANA_XTU,
	KANA_XYA,
	KANA_XYU,
	KANA_XYO,
	KANA_XWA,
	KANA_WYI, //ゐ
	KANA_WYE, //ゑ
	// 記号
	KANA_COMM,
	KANA_DOT,
	KANA_ZSLSH, //・
	KANA_ZDOT, //…
	KANA_ZCOMM, //‥
	KANA_ZMINS, //〜
	KANA_ZH, //←
	KANA_ZJ,
	KANA_ZK,
	KANA_ZL,
	KANA_ZLBRC, //『
	KANA_ZRBRC,
};

#endif // kana_h_INCLUDED
