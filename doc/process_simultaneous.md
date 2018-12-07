## important variable
simultaneousing_key: the key waiting simulaneous handling

waiting_buffer: simultaneous mods waiting simulaneous handling

pressed_time != 0 & released_time = 0 being pressed

pressed_time != 0 & released_time != 0 pressed and released but not processed

pressed_time == 0 already processed press but release by waiting_buffer_scan()

after processed both press and release, delete from buffer by waiting_buffer_del()

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
			waiting_buffer_scan_simultaneous
		assign simultaneousing_key
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
			release key
			delete from buffer
		if while simultaneous
			flag released
		else
			if there's only same key in buffer
				if within tapping term
					tap mod as key(not mod)
				else
					tap mod as mod
				delete from buffer
			if two simultaneous key is pressed
				determine which key is mod
				waiting_buffer_scan_simultaneous
			else
				waiting_buffer_scan_simultaneous
end
```

use waiting_buffer_scan_simultaneous
with release events which need simultaneous handling.
otherwise use waiting_buffer_scan
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

```
matrix_scan_simultaneous
	handle key repeat
	handle mod press
```
