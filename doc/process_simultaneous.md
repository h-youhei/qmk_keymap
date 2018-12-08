## important variable
`simultaneousing_key` is the key waiting simulaneous handling

`waiting_buffer` is simultaneous mods waiting simulaneous handling

### those variable's fields
`pressed_time != 0 & released_time == 0` means it is being pressed

`pressed_time != 0 & released_time != 0` means it is pressed and released but not processed by `waiting_buffer_scan()s`

`pressed_time == 0` means it is already processed press but release by `waiting_buffer_scan()s`

after processed both press and release, it is deleted from buffer by `waiting_buffer_del()`

## core logic
```
process_simultaneous
	if keycode is simultaneous mod or layer
		process_simultaneous_mod
	else
		process_simultaneous_key

	last some lines handle is_simultaneous_key
end
```

```
process_simultaneous_key
	if pressed
		if previous key waits simultaneous handling
			flag previous key released
			process waiting_buffer_scan as previous key is simultaneousing key
		assign new key to simultaneousing_key
	if released
		if repeating
			release key
			return
		flag released
		waiting_buffer_scan_simultaneous
end
```

```
process_simultaneous_mod
	if pressed
		add to buffer
	if released
		if already processed by waiting_buffer_scan
			return
		if already processed press event but release event by waiting_buffer_scan
			release mod
			delete from buffer
		if while simultaneous
			flag released
		else
			if there's only same key in buffer
				if within tapping term
					tap mod as key(not mod)
				else
					release mod
				delete from buffer
			if two simultaneous key is pressed
				determine which key is mod
				waiting_buffer_scan_simultaneous
			else
				waiting_buffer_scan_simultaneous
end
```

use `waiting_buffer_scan_simultaneous()`
with release events which need simultaneous handling.
otherwise use `waiting_buffer_scan()`
```
waiting_buffer_scan_simultaneous
	key_queue: tap mods as key at the end
	mod_stack: release mods at the end
	for loop on buffer
		if mod is pressed before key
			if mod is being pressed
				press mod
				flag pressed
			if mod is already released
				if is simultaneous
					press mod
					release mod at the end
				else
					tap mod as key
		if mod is pressed after key
			if mod is being pressed
				if is simulaneous
					press mod
					flag pressed
				else
					tap mod as key at the end
			if mod is already released
				if is simultaneous
					press mod
					release mod at the end
				else
					tap mod as key
	tap simultaneous key
	tap mod as key in key_queue
	release mod in mod_stack
end
```
## repeat and hold handling
`await_repeat` is if normal key is being pressed.<br>
`await_hold` is if simulaneous mod is being pressed.

reset `await_repeat` and `await_hold` every key press at `process_simultaneous()`'s beggining.

when normal key is pressed, `await_repeat` is set in `process_simultaneous_key()`

when simulaneous mod is pressed, `await_hold` is set in `process_simultaneous_mod()`

unregistering is handled by `waiting_buffer_scan_simultaneous()`

```
matrix_scan_simultaneous
	if normal key is being pressed
		if hold it for SIMULTANEOUS_WAIT_TERM or longer
			register mod in waiting_buffer and leave repeating to OS.
			this ifelse branch is to handle custom keycode in layer.
			register key
			prepare to unregister key correctly
			reset await_repeat
	if simultaneous mod is being pressed
		if hold it for TAPPING_TERM or longer,
		register mod and prepare to unregister key correctly by waiting_buffer_scan()
		reset await_hold
```

