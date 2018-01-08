#include "process_simultaneous.h"
#include "keycode.h" //KC_*
#include "action.h" //register/unregister_code/mods clear_keyboard
#include "action_tapping.h" //TAPPING_TERM WAITING_BUFFER_SIZE
#include "action_layer.h" //layer_state default_layer_state layer_switch_get_layer
#include "keymap.h" //keymap_key_to_keycode
#include "timer.h" //timer_elapsed
#include "util.h" //biton32
#include "util_user.h" //tap_code in_range
#include <stdlib.h> //malloc free

#include "nodebug.h"

typedef struct {
  keypos_t key;
  uint16_t pressed_time;
  uint16_t released_time;
} simultaneous_t;

typedef struct _simultaneous_node {
  simultaneous_t data;
  struct _simultaneous_node *prev;
  struct _simultaneous_node *next;
} simultaneous_node_t;

static simultaneous_t simultaneousing_key = {};
static simultaneous_node_t *waiting_buffer = NULL;

static void waiting_buffer_add(keyevent_t event);
static void waiting_buffer_del(simultaneous_node_t *node);
static simultaneous_node_t *waiting_buffer_get(keypos_t key);
static uint8_t waiting_buffer_length(void);
static void waiting_buffer_clear(void);
static void waiting_buffer_scan(void);
static void waiting_buffer_scan_simultaneous(void);

typedef struct _repeating_node {
  keypos_t key;
  struct _repeating_node *prev;
  struct _repeating_node *next;
} repeating_node_t;

static repeating_node_t *repeating_keys = NULL;
static bool await_repeat = false;

static void repeating_keys_add(keypos_t key);
static void repeating_keys_del(repeating_node_t *node);
static repeating_node_t *repeating_keys_get(keypos_t key);
static void repeating_keys_clear(void);

static void register_mod_or_layer_on(uint16_t keycode);
static void unregister_mod_or_layer_off(uint16_t keycode);
static bool is_simultaneous(simultaneous_t simultaneous_mod);
static bool is_simultaneous_mod(uint16_t keycode);
static bool is_simultaneous_layer(uint16_t keycode);
static bool process_simultaneous_key(uint16_t keycode, keyrecord_t *record);
static bool process_simultaneous_mod(uint16_t keycode, keyrecord_t *record);

static inline bool waiting_buffer_only(simultaneous_node_t node) {
  return node.prev == NULL && node.next == NULL;
}
static inline uint16_t timer_diff(uint16_t timer_a, uint16_t timer_b) {
  return timer_a >= timer_b ? timer_a - timer_b : timer_b - timer_a;
}
static inline uint16_t get_keycode_from_keypos(keypos_t keypos) {
  return keymap_key_to_keycode(layer_switch_get_layer(keypos), keypos);
}
static inline uint8_t get_key_from_keycode(uint16_t keycode) {
  return keycode & 0xFF;
}
static inline uint8_t get_mod_from_keycode(uint16_t keycode) {
  return (keycode ^ USER_MOD_SIMULTANEOUS) >> 8;
}
static inline uint8_t mod_for_send(uint8_t mod) {
  return mod&0x10 ? mod<<4 : mod;
}
static inline uint8_t get_layer_from_keycode(uint16_t keycode) {
  return (keycode ^ USER_LAYER_SIMULTANEOUS) >> 8;
}
static inline uint8_t swap_mod(uint8_t mod) {
  return mod ^ 0x10;
}
static inline bool within_tapping_term(uint16_t time) {
  return timer_elapsed(time) < TAPPING_TERM;
}
static inline bool within_simultaneousing_term(uint16_t time) {
  return timer_elapsed(time) < SIMULTANEOUS_WAIT_TERM;
}
static inline bool is_simltaneousing(simultaneous_t simultaneous) {
  return simultaneous.pressed_time >= 0;
}
static inline void clear_simultaneousing_key(void) {
  simultaneousing_key.pressed_time = 0;
  simultaneousing_key.released_time = 0;
}

__attribute__ ((weak))
bool is_simultaneous_key(uint16_t keycode) {
  return in_range(keycode, KC_A, KC_0) || in_range(keycode, KC_MINUS, KC_SLASH);
}

__attribute__ ((weak))
bool has_simultaneous_priority_to_a (uint16_t keycode_a, uint16_t keycode_b) {
  uint8_t mod_a = get_mod_from_keycode(keycode_a);
  uint8_t mod_b = get_mod_from_keycode(keycode_b);
  if(mod_a == MOD_LSFT) return true;
  if(mod_a == MOD_RSFT && mod_b != MOD_LSFT) return true;
  return false;
}

void waiting_buffer_add(keyevent_t event) {
  if(IS_NOEVENT(event)) return;

  simultaneous_node_t *p = malloc(sizeof(simultaneous_node_t));
  if(p == NULL) {
	clear_keyboard();
	clear_simultaneousing_key();
	waiting_buffer_clear();
	repeating_keys_clear();
  }
  else {
	p->data.key = event.key;
	p->data.pressed_time = event.time;
	p->data.released_time = 0;
	p->next = NULL;
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
  if(next != NULL) next->prev = prev;
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
}

void waiting_buffer_scan() {
  for(simultaneous_node_t *p = waiting_buffer; p!= NULL; p = p->next) {
	uint16_t keycode = get_keycode_from_keypos(p->data.key);
	if(p->data.released_time == 0) {
	  register_mod_or_layer_on(keycode);
	  p->data.pressed_time = 0;
	}
	else {
	  tap_code(get_key_from_keycode(keycode));
	  waiting_buffer_del(p);
	}
  }
}

void waiting_buffer_scan_simultaneous() {
  uint16_t key_queue[WAITING_BUFFER_SIZE] = {};
  uint16_t mod_stack[WAITING_BUFFER_SIZE] = {};
  uint8_t key_queue_head = 0;
  uint8_t mod_stack_head = 0;

  /* if(waiting_buffer == NULL) { */
	/* error("waiting_buffer shouldn't NULL in waiting_buffer_scan()"); */
  /* } */
  for(simultaneous_node_t *p = waiting_buffer; p != NULL; p = p->next) {
	uint16_t keycode = get_keycode_from_keypos(p->data.key);
	/* mod is pressed before key */
	if(p->data.pressed_time < simultaneousing_key.pressed_time) {
	  /* mod is being pressed */
	  if(p->data.released_time == 0) {
		register_mod_or_layer_on(keycode);
		p->data.pressed_time = 0;
	  }
	  /* mod is already released */
	  else {
		if(is_simultaneous(p->data)) {
		  register_mod_or_layer_on(keycode);
		  mod_stack[mod_stack_head++] = keycode;
		}
		else {
		  tap_code(get_key_from_keycode(keycode));
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
		  p->data.pressed_time = 0;
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
  }
  uint16_t keycode = get_keycode_from_keypos(simultaneousing_key.key);
  if(is_simultaneous_mod(keycode) || is_simultaneous_layer(keycode)) {
	tap_code(get_key_from_keycode(keycode));
  }
  else tap_code(keycode);
  clear_simultaneousing_key();
  for(uint8_t i = 0; i < key_queue_head; i++) {
	tap_code(get_key_from_keycode(key_queue[i]));
  }
  for(uint8_t i = mod_stack_head; i > 0;) {
	unregister_mod_or_layer_off(mod_stack[--i]);
  }
}

void repeating_keys_add(keypos_t key) {
  repeating_node_t *p = malloc(sizeof(repeating_node_t));
  if(p == NULL) {
	clear_keyboard();
	clear_simultaneousing_key();
	waiting_buffer_clear();
	repeating_keys_clear();
  }
  else {
	p->key = key;
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
	register_mods(mod_for_send(get_mod_from_keycode(keycode)));
  }
  else {
	layer_on(get_layer_from_keycode(keycode));
  }
}

void unregister_mod_or_layer_off(uint16_t keycode) {
  if(is_simultaneous_mod(keycode)) {
	unregister_mods(mod_for_send(get_mod_from_keycode(keycode)));
  }
  else {
	layer_off(get_layer_from_keycode(keycode));
  }
}
bool is_simultaneous(simultaneous_t simultaneous_mod) {
  if(simultaneous_mod.released_time == 0) {
	simultaneous_mod.released_time = simultaneousing_key.released_time;
  }

#ifdef SIMULTANEOUS_ALLOW_PERCENTAGE
  if(simultaneousing_key.pressed_time > simultaneous_mod.pressed_time) {
	simultaneous_mod.pressed_time = simultaneousing_key.pressed_time;
  }
  return (float)timer_diff(simultaneous_mod.pressed_time, simultaneous_mod.released_time) / timer_diff(simultaneousing_key.pressed_time, simultaneousing_key.released_time) >= (float)SIMULTANEOUS_ALLOW_PERCENTAGE / 100;

#else
  return timer_diff(simultaneousing_key.pressed_time, simultaneous_mod.pressed_time) <= SIMULTANEOUS_WAIT_TERM && timer_diff(simultaneousing_key.released_time, simultaneous_mod.released_time) <= SIMULTANEOUS_WAIT_TERM;

#endif
  return false;
}

bool process_simultaneous(uint16_t keycode, keyrecord_t *record) {
  await_repeat = false;

  if(is_simultaneous_mod(keycode) || is_simultaneous_layer(keycode)) {
	return process_simultaneous_mod(keycode, record);
  }
  else if(is_simultaneous_key(keycode)) {
	return process_simultaneous_key(keycode, record);
  }
  else {
	keyevent_t event = record->event;
	if(event.pressed) {
	  if(simultaneousing_key.pressed_time != 0) {
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
	if(simultaneousing_key.pressed_time != 0) {
	  simultaneousing_key.released_time = event.time;
	  waiting_buffer_scan_simultaneous();
	}

	simultaneousing_key.key = event.key;
	simultaneousing_key.pressed_time = event.time;
	simultaneousing_key.released_time = 0;
	await_repeat = true;
  }
  else {
	repeating_node_t *repeating_p = repeating_keys_get(event.key);
	if(repeating_p != NULL) {
	  repeating_keys_del(repeating_p);
	  unregister_code(get_keycode_from_keypos(event.key));
	  return false;
	}

	if(!KEYEQ(event.key, simultaneousing_key.key)) return true;

	// already released in waiting_buffer_scan()
	if(simultaneousing_key.pressed_time == 0) return false;

	if(waiting_buffer == NULL) {
	  tap_code(keycode);
	  clear_simultaneousing_key();
	}
	else {
	  simultaneousing_key.released_time = event.time;
	  waiting_buffer_scan_simultaneous();
	}
  }
  return false;
}

bool process_simultaneous_mod(uint16_t keycode, keyrecord_t *record) {
  keyevent_t event = record->event;
  if(event.pressed) {
	waiting_buffer_add(event);
  }
  else {
	simultaneous_node_t* p = waiting_buffer_get(event.key);
	/* pressed and released in waiting_buffer_scan() */
	if(p == NULL) return false;
	/* pressed in waiting_buffer_scan() */
	if(p->data.pressed_time == 0) {
	  unregister_mod_or_layer_off(keycode);
	  waiting_buffer_del(p);
	}
	else if(simultaneousing_key.pressed_time > 0) {
		p->data.released_time = event.time;
	}
	else {
	  if(waiting_buffer_only(*p)) {
		if(within_tapping_term(p->data.pressed_time)) {
		  tap_code(get_key_from_keycode(keycode));
		}
		else {
		  register_mod_or_layer_on(keycode);
		  unregister_mod_or_layer_off(keycode);
		}
		waiting_buffer_del(p);
	  }
	  else if(waiting_buffer_length() == 2) {
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
		waiting_buffer_del(p);
		waiting_buffer_scan();
		tap_code(get_key_from_keycode(keycode));
	  }
	}
  }
  return false;
}

void matrix_scan_simultaneous() {
  if(await_repeat) {
	if(!within_simultaneousing_term(simultaneousing_key.pressed_time)) {
	  waiting_buffer_scan();
	  register_code(get_keycode_from_keypos(simultaneousing_key.key));
	  repeating_keys_add(simultaneousing_key.key);
	  await_repeat = false;
	  clear_simultaneousing_key();
	}
  }
}
