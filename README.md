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
|      | Q | C | L | P | V | E |                   | I | K | G | U | Y | Z |      |
|      |   |   |   |   |   | s |                   | n |   |   |   |   |   |      |
|--------------------------| c |                   | s |--------------------------|
|   ?  | O | S | R | N | H |___|                   |___| D | T | E | I | A |  :   |
|   /  |   |   |   |   |   |   |                   |   |   |   |   |   |   |  ;   |
|--------------------------| B |                   | D |--------------------------|
|      | X | J | _ | F | B | S |                   | e | W | M | < | > | " |      |
|      |   |   | - |   |   |   | -------   ------- | l |   |   | , | . | ' |      |
------------------------------- | R | L | |Ba |For| ------------------------------
   |Win|Ctl| ~ | \ | + |        |Btn|Btn| | ck| wd|        | { | } |Pau|Alt|Sft|
   |Lck|Lck| ` | | | = |     -----------| |-----------     | [ | ] | se|Lck|Lck|
   |   |   |Win|Ctl|Alt|    |Sft|Fn | M | |   |Fn |Sft|    |Alt|Ctl|Win|   |   |
    -------------------     |   |   |Btn| |HP |   |   |     -------------------
                            | S | E |---| |---| T | S |
                            | p | n |Cap| |IME| a | p |
                            | c | t |Lck| |TGL| b | c |
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

To use it,<br>
1. copy `process_simultaneous.c/h` and `util_user.c/h`, and append it to SRC in your `rules.mk`
2. include `process_simultaneous.h` to your `keymap.c`
3. call `process_simultaneous()` from `process_record_user()`
4. call `matrix_scan_simultaneous()` from `matrix_scan_user()`

To customize, read process_simultaneous.h .

## Special Keys
WinLck,CtrLck,AltLck,SftLck are modifier lock like CapsLock.

Clear key is used to unlock modifier lock all at once.

## details
IME key is used for toggle Input Method. Internally, it's assigned to KANA key.

## Japanese Input
see [Japanese Readme](README.ja.md)

[qmk]: https://github.com/qmk/qmk_firmware
