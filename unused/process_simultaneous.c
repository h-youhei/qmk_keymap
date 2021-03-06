#include "process_simultaneous.h"
#include "keycode.h" //KC_*
#include "action.h" //register/unregister/tap_code/mods clear_keyboard
#include "action_tapping.h" //TAPPING_TERM WAITING_BUFFER_SIZE
#include "action_layer.h" //layer_on layer_off
#include "timer.h" //timer_elapsed TIMER_DIFF_16
#include "util_user.h" //in_range get_keycode_from_keypos mod_for_send
#include <stdlib.h> //malloc free

#include "nodebug.h"

typedef struct {
	keypos_t key;
	uint16_t pressed_time;
	uint16_t released_time;
	// wether press is already processed
	bool processed;
} simultaneous_t;

typedef struct _simultaneous_node {
	simultaneous_t data;
	struct _simultaneous_node *prev;
	struct _simultaneous_node *next;
} simultaneous_node_t;

static bool process_simultaneous_key(uint16_t keycode, keyrecord_t *record);
static bool process_simultaneous_mod(uint16_t keycode, keyrecord_t *record);

static simultaneous_t simultaneousing_key = {};
static void clear_simultaneousing_key(void);

static simultaneous_node_t *waiting_buffer = NULL;
static simultaneous_node_t *waiting_buffer_last = NULL;
static void waiting_buffer_add(keyevent_t event);
static void waiting_buffer_del(simultaneous_node_t *node);
static simultaneous_node_t *waiting_buffer_get(keypos_t key);
static uint8_t waiting_buffer_length(void);
static void waiting_buffer_clear(void);
static void waiting_buffer_scan(void);
static void waiting_buffer_scan_simultaneous(void);
static bool is_simultaneous(simultaneous_t simultaneous_mod);

typedef struct _repeating_node {
	keypos_t key;
	uint16_t keycode;
	struct _repeating_node *prev;
	struct _repeating_node *next;
} repeating_node_t;

static repeating_node_t *repeating_keys = NULL;
static bool await_repeat = false;
static bool await_hold = false;

static void repeating_keys_add(keypos_t key);
static void repeating_keys_del(repeating_node_t *node);
static repeating_node_t *repeating_keys_get(keypos_t key);
static void repeating_keys_clear(void);

static void register_simultaneous(void);
static void unregister_simultaneous(void);
static void tap_simultaneous(void);
static void register_mod_or_layer_on(uint16_t keycode);
static void unregister_mod_or_layer_off(uint16_t keycode);
static void tap_simultaneous_mod(uint16_t keycode);

static void clear_global(void);
static uint8_t get_key_from_keycode(uint16_t keycode);
static bool within_tapping_term(uint16_t time);

static inline bool IS_SIMULTANEOUSING(void) {
	return simultaneousing_key.pressed_time > 0;
}
static inline bool WITHIN_SIMULTANEOUS_WAIT_TERM(void) {
	return timer_elapsed(simultaneousing_key.pressed_time) < SIMULTANEOUS_WAIT_TERM;
}

__attribute__((weak))
bool is_simultaneous_key(uint16_t keycode) {
	return IS_KEY(keycode);
}

__attribute__((weak))
bool has_simultaneous_priority_to_a(uint16_t keycode_a, uint16_t keycode_b) {
	if(is_simultaneous_mod(keycode_a)) {
		uint8_t mod_a = get_simultaneous_mod_from_keycode(keycode_a);
		if(mod_a == MOD_LSFT) return true;
		else if(mod_a == MOD_RSFT) {
			if(is_simultaneous_mod(keycode_b)) {
				uint8_t mod_b = get_simultaneous_mod_from_keycode(keycode_b);
				return mod_b != MOD_LSFT;
			}
			else return true;
		}
	}
	return false;
}

static bool ignore_process_simultaneous = false;
bool process_simultaneous(uint16_t keycode, keyrecord_t *record) {
	if(ignore_process_simultaneous) return true;
	await_repeat = false;
	await_hold = false;

	if(is_simultaneous_mod(keycode) || is_simultaneous_layer(keycode)) {
		return process_simultaneous_mod(keycode, record);
	}
	else if(is_simultaneous_key(keycode)) {
		return process_simultaneous_key(keycode, record);
	}
	else {
		keyevent_t event = record->event;
		if(event.pressed) {
			if(IS_SIMULTANEOUSING()) {
				simultaneousing_key.released_time = event.time;
				waiting_buffer_scan_simultaneous();
			}
			waiting_buffer_scan();
		}
		return true;
	}
}

bool process_simultaneous_key(uint16_t keycode, keyrecord_t *record) {
	keyevent_t event = record->event;

	if(event.pressed) {
		// prev_key
		if(IS_SIMULTANEOUSING()) {
			simultaneousing_key.released_time = event.time;
			waiting_buffer_scan_simultaneous();
		}

		simultaneousing_key.key = event.key;
		simultaneousing_key.pressed_time = event.time;
		simultaneousing_key.released_time = 0;
		simultaneousing_key.processed = false;
		if(IS_KEY(keycode)) await_repeat = true;
	}
	else {
		repeating_node_t *repeating_p = repeating_keys_get(event.key);
		if(repeating_p != NULL) {
			unregister_code(repeating_p->keycode);
			repeating_keys_del(repeating_p);
			return false;
		}

		if(!KEYEQ(event.key, simultaneousing_key.key)) return true;

		if(simultaneousing_key.processed) return false;

		simultaneousing_key.released_time = event.time;
		waiting_buffer_scan_simultaneous();
	}
	return false;
}

bool process_simultaneous_mod(uint16_t keycode, keyrecord_t *record) {
	keyevent_t event = record->event;
	if(event.pressed) {
		waiting_buffer_add(event);
		await_hold = true;
	}
	else {
		simultaneous_node_t* p = waiting_buffer_get(event.key);
		// pressed and released in waiting_buffer_scan()
		if(p == NULL) return false;
		// pressed in waiting_buffer_scan()
		if(p->data.processed) {
			unregister_mod_or_layer_off(keycode);
			waiting_buffer_del(p);
		}
		else if(IS_SIMULTANEOUSING()) {
			// mark released to process in waiting_buffer_scan_simultaneous()
			p->data.released_time = event.time;
		}
		else {
			uint8_t length = waiting_buffer_length();
			if(length == 1) {
				if(within_tapping_term(p->data.pressed_time)) {
					tap_simultaneous_mod(keycode);
				}
				else {
					unregister_mod_or_layer_off(keycode);
				}
				waiting_buffer_del(p);
			}
			else if(length == 2) {
				p->data.released_time = event.time;
				simultaneous_node_t *another_p = p->prev;
				if(another_p == NULL) another_p = p->next;
				uint16_t another_keycode = get_keycode_from_keypos(another_p->data.key);
				if(has_simultaneous_priority_to_a(keycode, another_keycode)) {
					simultaneousing_key = another_p->data;
					simultaneousing_key.released_time = event.time;
					if(is_simultaneous(p->data)) {
						waiting_buffer_del(another_p);
						waiting_buffer_scan_simultaneous();
						return false;
					}
				}
				simultaneousing_key = p->data;
				waiting_buffer_del(p);
				waiting_buffer_scan_simultaneous();
			}
			else {
				p->data.released_time = event.time;
				simultaneousing_key = p->data;
				waiting_buffer_del(p);
				waiting_buffer_scan_simultaneous();
			}
		}
	}
	return false;
}

void matrix_scan_simultaneous() {
	if(await_repeat) {
		if(!WITHIN_SIMULTANEOUS_WAIT_TERM()) {
			waiting_buffer_scan();
			uint16_t keycode = get_keycode_from_keypos(simultaneousing_key.key);
			// special keycode might not be repeatable
			if(IS_KEY(keycode)) {
				register_code(keycode);
				repeating_keys_add(simultaneousing_key.key);
			}
			else register_simultaneous();
			await_repeat = false;
			clear_simultaneousing_key();
		}
	}
	if(await_hold) {
		uint16_t pressed_time = waiting_buffer_last->data.pressed_time;
		if(!within_tapping_term(pressed_time)) {
			waiting_buffer_scan();
			await_hold = false;
		}
	}
}

void clear_simultaneousing_key(void) {
	simultaneousing_key.pressed_time = 0;
	simultaneousing_key.released_time = 0;
}

void waiting_buffer_add(keyevent_t event) {
	if(IS_NOEVENT(event)) return;

	simultaneous_node_t *p = malloc(sizeof(simultaneous_node_t));
	if(p == NULL) {
		clear_keyboard();
		clear_global();
	}
	else {
		p->data.key = event.key;
		p->data.pressed_time = event.time;
		p->data.released_time = 0;
		p->data.processed = false;
		p->next = NULL;
		waiting_buffer_last = p;
		if(waiting_buffer == NULL) {
			p->prev = NULL;
			waiting_buffer = p;
		}
		else {
			simultaneous_node_t *last = waiting_buffer;
			for(;last->next != NULL; last = last->next);
			p->prev = last;
			last->next = p;
		}
	}
}

void waiting_buffer_del(simultaneous_node_t *node) {
	simultaneous_node_t *prev = node->prev;
	simultaneous_node_t *next = node->next;
	if(next == NULL) waiting_buffer_last = NULL;
	else next->prev = prev;
	if(prev == NULL) waiting_buffer = next;
	else node->prev->next = next;
	free(node);
}

simultaneous_node_t *waiting_buffer_get(keypos_t key) {
	for(simultaneous_node_t *p = waiting_buffer; p != NULL; p = p->next) {
		if(KEYEQ(p->data.key, key)) return p;
	}
	return NULL;
}

uint8_t waiting_buffer_length() {
	uint8_t i = 0;
	for(simultaneous_node_t *p = waiting_buffer; p != NULL; p = p->next) {
		i++;
	}
	return i;
}

void waiting_buffer_clear() {
	for(simultaneous_node_t *p = waiting_buffer; p != NULL;) {
		simultaneous_node_t *next = p->next;
		free(p);
		p = next;
	}
	waiting_buffer = NULL;
	waiting_buffer_last = NULL;
}

void waiting_buffer_scan() {
	for(simultaneous_node_t *p = waiting_buffer; p!= NULL; p = p->next) {
		uint16_t keycode = get_keycode_from_keypos(p->data.key);
		/* mod is being pressed */
		if(p->data.released_time == 0) {
			register_mod_or_layer_on(keycode);
			p->data.processed = true;
		}
		else {
			tap_simultaneous_mod(keycode);
			waiting_buffer_del(p);
		}
	}
}

void waiting_buffer_scan_simultaneous() {
	// tap mod as key at the end
	uint16_t key_queue[WAITING_BUFFER_SIZE] = {};
	// release mods at the end
	uint16_t mod_stack[WAITING_BUFFER_SIZE] = {};
	uint8_t key_queue_head = 0;
	uint8_t mod_stack_head = 0;

	for(simultaneous_node_t *p = waiting_buffer; p != NULL; p = p->next) {
		uint16_t keycode = get_keycode_from_keypos(p->data.key);
		/* mod is pressed before key */
		if(p->data.pressed_time < simultaneousing_key.pressed_time) {
			/* mod is being pressed */
			if(p->data.released_time == 0) {
				register_mod_or_layer_on(keycode);
				p->data.processed = true;
			}
			/* mod is already released */
			else {
				if(is_simultaneous(p->data)) {
					register_mod_or_layer_on(keycode);
					mod_stack[mod_stack_head++] = keycode;
				}
				else {
					tap_simultaneous_mod(keycode);
				}
				waiting_buffer_del(p);
			}
		}
		/* mod is pressed after key */
		else {
			/* mod is being pressed */
			if(p->data.released_time == 0) {
				if(is_simultaneous(p->data)) {
					register_mod_or_layer_on(keycode);
					p->data.processed = true;
				}
				else {
					key_queue[key_queue_head++] = keycode;
					waiting_buffer_del(p);
				}
			}
			/* mod is already released */
			else {
				if(is_simultaneous(p->data)) {
					register_mod_or_layer_on(keycode);
					mod_stack[mod_stack_head++] = keycode;
				}
				else {
					key_queue[key_queue_head++] = keycode;
				}
				waiting_buffer_del(p);
			}
		}
		if(key_queue_head >= WAITING_BUFFER_SIZE || mod_stack_head >= WAITING_BUFFER_SIZE) {
			clear_keyboard();
			clear_global();
			return;
		}
	}
	uint16_t keycode = get_keycode_from_keypos(simultaneousing_key.key);
	if(is_simultaneous_mod(keycode) || is_simultaneous_layer(keycode)) {
		tap_simultaneous_mod(keycode);
	}
	else tap_simultaneous();
	clear_simultaneousing_key();
	for(uint8_t i = 0; i < key_queue_head; i++) {
		tap_simultaneous_mod(key_queue[i]);
	}
	for(uint8_t i = mod_stack_head; i > 0;) {
		unregister_mod_or_layer_off(mod_stack[--i]);
	}
}

void repeating_keys_add(keypos_t key) {
	repeating_node_t *p = malloc(sizeof(repeating_node_t));
	if(p == NULL) {
		clear_keyboard();
		clear_global();
	}
	else {
		p->key = key;
		p->keycode = get_keycode_from_keypos(key);
		p->next = NULL;
		if(repeating_keys == NULL) {
			p->prev = NULL;
			repeating_keys = p;
		}
		else {
			repeating_node_t *last = repeating_keys;
			for(;last->next != NULL; last = last->next);
			p->prev = last;
			last->next = p;
		}
	}
}

void repeating_keys_del(repeating_node_t *node) {
	repeating_node_t *prev = node->prev;
	repeating_node_t *next = node->next;
	if(next != NULL) next->prev = prev;
	if(prev == NULL) repeating_keys = next;
	else node->prev->next = next;
	free(node);
}

repeating_node_t *repeating_keys_get(keypos_t key) {
	for(repeating_node_t *p = repeating_keys; p != NULL; p = p->next) {
		if(KEYEQ(p->key, key)) return p;
	}
	return NULL;
}

void repeating_keys_clear() {
	for(repeating_node_t *p = repeating_keys; p != NULL;) {
		repeating_node_t *next = p->next;
		free(p);
		p = next;
	}
	repeating_keys = NULL;
}

bool is_simultaneous_mod(uint16_t keycode) {
	return in_range(keycode, USER_MOD_SIMULTANEOUS, USER_MOD_SIMULTANEOUS_MAX);
}

bool is_simultaneous_layer(uint16_t keycode) {
	return in_range(keycode, USER_LAYER_SIMULTANEOUS, USER_LAYER_SIMULTANEOUS_MAX);
}

void register_mod_or_layer_on(uint16_t keycode) {
	if(is_simultaneous_mod(keycode)) {
		register_mods(mod_for_send(get_simultaneous_mod_from_keycode(keycode)));
	}
	else {
		layer_on(get_simultaneous_layer_from_keycode(keycode));
	}
}

void unregister_mod_or_layer_off(uint16_t keycode) {
	if(is_simultaneous_mod(keycode)) {
		unregister_mods(mod_for_send(get_simultaneous_mod_from_keycode(keycode)));
	}
	else {
		layer_off(get_simultaneous_layer_from_keycode(keycode));
	}
}

void register_simultaneous() {
	ignore_process_simultaneous = true;
	process_record(&(keyrecord_t) {
		.event.key = simultaneousing_key.key,
		.event.time = simultaneousing_key.pressed_time,
		.event.pressed = true,
		.simultaneous = true
	});
	ignore_process_simultaneous = false;
}

void unregister_simultaneous() {
	ignore_process_simultaneous = true;
	process_record(&(keyrecord_t) {
		.event.key = simultaneousing_key.key,
		.event.time = simultaneousing_key.released_time,
		.event.pressed = false,
		.simultaneous = true
	});
	ignore_process_simultaneous = false;
}

void tap_simultaneous() {
	register_simultaneous();
	unregister_simultaneous();
}

void tap_simultaneous_mod(uint16_t keycode) {
	tap_code(get_key_from_keycode(keycode));
}

bool is_simultaneous(simultaneous_t simultaneous_mod) {
//{ check if both key is pressed whithin SIMULTANEOUS_WAIT_TERM
	uint16_t pressed_first;
	uint16_t pressed_second;
	if(simultaneous_mod.pressed_time < simultaneousing_key.pressed_time) {
		pressed_first = simultaneous_mod.pressed_time;
		pressed_second = simultaneousing_key.pressed_time;
	}
	else {
		pressed_first= simultaneousing_key.pressed_time;
		pressed_second= simultaneous_mod.pressed_time;
	}
	if(TIMER_DIFF_16(pressed_second, pressed_first) > SIMULTANEOUS_WAIT_TERM) {
		return false;
	}
//}
//{ check if both key is held over SIMULTANEOUSING_TERM
	uint16_t released_first;
	// mod is being pressed
	if(simultaneous_mod.released_time == 0) {
		released_first = simultaneousing_key.released_time;
	}
	else if(simultaneous_mod.released_time < simultaneousing_key.released_time) {
		released_first = simultaneous_mod.released_time;
	}
	else {
		released_first = simultaneousing_key.released_time;
	}
	return(TIMER_DIFF_16(released_first, pressed_second) > SIMULTANEOUSING_TERM);
//}
}

void clear_global() {
	clear_simultaneousing_key();
	waiting_buffer_clear();
	repeating_keys_clear();
}

uint8_t get_key_from_keycode(uint16_t keycode) {
	return keycode & 0xFF;
}

uint8_t get_simultaneous_mod_from_keycode(uint16_t keycode) {
	return (keycode ^ USER_MOD_SIMULTANEOUS) >> 8;
}

uint8_t get_simultaneous_layer_from_keycode(uint16_t keycode) {
	return (keycode ^ USER_LAYER_SIMULTANEOUS) >> 8;
}

bool within_tapping_term(uint16_t time) {
	return timer_elapsed(time) < TAPPING_TERM;
}
