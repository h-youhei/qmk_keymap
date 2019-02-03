# Youhei's keymap
English|[日本語](README.ja.md)

## Install
1. clone [qmk_firmware][qmk]
2. clone this repository in `qmk_firmware/layouts/community/ergodox`
3. run `sh write.sh`

## Base
```
 ------------------------------                     ------------------------------
| Prnt | # | @ | ! | % | $ |   |                   |   | * | ( | ) | ^ | & |Clear |
|  Scr | 3 | 2 | 1 | 5 | 4 |   |                   |   | 8 | 9 | 0 | 6 | 7 |      |
|------------------------------|                   |------------------------------|
|      | Q | C | L | P | J |Cap|                   |   | K | F | U | Y | Z |      |
|      |   |   |   |   |   |Lck|                   |Ins|   |   |   |   |   |      |
|--------------------------|   |                   |   |--------------------------|
|   ?  | O | S | R | N | M |___|                   |___| G | T | E | I | A |  :   |
|   /  |   |   |   |   |   |   |                   |   |   |   |   |   |   |  ;   |
|--------------------------|BS |                   |Del|--------------------------|
|      | X | V | _ | H | B |   |                   |   | W | D | < | > | " |      |
|      |   |   | - |   |   |   | -------   ------- |   |   |   | , | . | ' |      |
------------------------------- | R | L | |Ba |For| ------------------------------
   |   |   | ~ | \ | + |        |Btn|Btn| | ck| wd|        | { | } |Pau|   |   |
   |   |   | ` | | | = |     -----------| |-----------     | [ | ] | se|   |   |
   |   |   |Win|Ctl|Alt|    |Sft|Fn | M | |   |Fn |Sft|    |Alt|Ctl|Win|   |   |
    -------------------     |   |   |Btn| |HP |   |   |     -------------------
                            | S | E |---| |---| T | S |
                            | p | n |Esc| |IME| a | p |
                            | c | t |   | |TGL| b | c |
                             -----------   -----------
```

## Fn layer

```
 ------------------------------                     ------------------------------
|      |   |   |   |   |   |   |                   |   |   |   |   |   |   |      |
|      |F3 |F2 |F1 |F5 |F4 |   |                   |   |F8 |F9 |F10|F6 |F7 |      |
|------------------------------|                   |------------------------------|
|      |   |   |   |   |Num|   |                   |   |   |Le |Ri |   |   |      |
|      |   | 6 | 5 | 4 |Lk |   |                   |   |   |ft |ght|F11|F12|      |
|--------------------------|   |                   |   |--------------------------|
|      |   |   |   |   |   |___|                   |___|Ho |Do |   |   |   |      |
|      | . | 3 | 2 | 1 | 0 |   |                   |   |me |wn |Up |End|App|      |
|--------------------------|   |                   |   |--------------------------|
|      |   |   |   |   |   |   |                   |   |   |Pg |Pg |Scr|   |      |
|      |   | 9 | 8 | 7 |000|   | -------   ------- |   |   |Dn |Up |Lck|   |      |
------------------------------- |Vol|Vol| |   |   | ------------------------------
   |   |   |   |   |   |        | Dn| Up| |   |   |        |   |   |   |   |   |
   |   |   |   |   |   |     -----------| |-----------     |   |   |   |   |   |
   |   |   |Win|Ctl|Alt|    |Sft|Fn |Mu | |   |Fn |Sft|    |Alt|Ctl|Win|   |   |
    -------------------     |   |   | te| |   | C |   |     -------------------
                            | S | E |---| |---| o | S |
                            | p | n |   | |   | n | p |
                            | c | t |   | |   | v | c |
                             -----------   -----------
```

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

## details
IME key is used for toggle Input Method. Internally, it's assigned to KANA key.

## Japanese Input
see [Japanese Readme](README.ja.md)

[qmk]: https://github.com/qmk/qmk_firmware
