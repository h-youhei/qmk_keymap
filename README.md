# keymap for qmk firmware
English|[日本語](README.ja.md)

## Install
1. clone [qmk_firmware][qmk]
2. clone this repository in `qmk_firmware/layouts/community/ergodox`
3. setup qmk environment ([official doc](https://docs.qmk.fm/#/newbs_getting_started?id=set-up-your-environment))
3. then build and flash ([official doc](https://docs.qmk.fm/#/newbs_building_firmware?id=build-your-firmware))

## Base
![ascii layout](img/ascii-layout.png)

## Num,Sign Layer
![num sign layout](img/num-sign-layout.png)

## Fn layer
![fn-layout](img/fn-layout.png)

<!--
## Simultaneous Modifier
simultaneous tapping version of MOD-TAP.

Key press is treated as simultaneous tapping
when satisfy both condition below.

- difference of pressed time is within the term defined (`SIMULTANEOUS_WAIT_TERM`)
- overlapping time the keys are held is over the term defined (`SIMULTANEOUSING_TERM`)

To use it,<br>
1. copy `process_simultaneous.c/h` and `util_user.c/h`, and append it to SRC in your `rules.mk`
2. include `process_simultaneous.h` to your `keymap.c`
3. call `process_simultaneous()` from `process_record_user()`
4. call `matrix_scan_simultaneous()` from `matrix_scan_user()`

and edit `qmk_firmware/tmk_core/common/action.h`
```
typedef struct {
	...
	bool simultaneous;
} keyrecord_t;
```

or if you don't need to change behavior in `process_record_user`,
depending on wether it is simultaneous tapping or not,
remove following code in `(un)register_simultaneous()` in `process_simultaneous.c`
```
process_record(...
	...,
	.simultaneous = true
	...
```

To customize, read process_simultaneous.h .
-->

## details
IME key is used for toggle Input Method.

## Japanese Input
see [Japanese Readme](README.ja.md)

[qmk]: https://github.com/qmk/qmk_firmware
