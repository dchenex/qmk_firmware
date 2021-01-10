 /* Copyright 2019 Thomas Baart <thomas@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum layers {
	_QWERTY = 0,
	_RAISE,
	_LOWER,
	_ADJUST,
	_GAME,
	_LEAGUE
};

typedef struct {
	bool is_press_action;
	uint8_t state;
} tap;

enum {
	SINGLE_TAP = 1,
	SINGLE_HOLD,
	DOUBLE_TAP,
	DOUBLE_HOLD,
	DOUBLE_SINGLE_TAP,
	TRIPLE_TAP,
	TRIPLE_HOLD
};

enum {
	L_BRK,
	R_BRK,
};

uint8_t cur_dance(qk_tap_dance_state_t *state);

// For the x tap dance. Put it here so it can be used in any keymap
void lbrk_finished(qk_tap_dance_state_t *state, void *user_data);
void lbrk_reset(qk_tap_dance_state_t *state, void *user_data);
void rbrk_finished(qk_tap_dance_state_t *state, void *user_data);
void rbrk_reset(qk_tap_dance_state_t *state, void *user_data);

uint8_t cur_dance(qk_tap_dance_state_t *state) {
	if (state->count == 1) {
		if (state->interrupted || !state->pressed) return SINGLE_TAP;
		else return SINGLE_HOLD;
	} else if (state->count == 2) {
		if (state->interrupted) return DOUBLE_SINGLE_TAP;
		else if (state->pressed) return DOUBLE_HOLD;
		else return DOUBLE_TAP;
	}

	if (state->count == 3) {
		if (state->interrupted || !state->pressed) return TRIPLE_TAP;
		else return TRIPLE_HOLD;
	} else return 8;
}

// Create an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {
	.is_press_action = true,
	.state = 0
};

void lbrk_finished(qk_tap_dance_state_t *state, void *user_data) {
	xtap_state.state = cur_dance(state);
	switch (xtap_state.state) {
		case SINGLE_TAP: register_code(KC_LSFT); register_code(KC_9); break;
		case DOUBLE_TAP: register_code(KC_LBRC); break;
		case TRIPLE_TAP: register_code(KC_LSFT); register_code(KC_LBRC); break;
	}
}

void lbrk_reset(qk_tap_dance_state_t *state, void *user_data) {
	switch (xtap_state.state) {
		case SINGLE_TAP: unregister_code(KC_LSFT); unregister_code(KC_9); break;
		case DOUBLE_TAP: unregister_code(KC_LBRC); break;
		case TRIPLE_TAP: unregister_code(KC_LSFT); unregister_code(KC_LBRC); break;
	}
	xtap_state.state = 0;
}

void rbrk_finished(qk_tap_dance_state_t *state, void *user_data) {
	xtap_state.state = cur_dance(state);
	switch (xtap_state.state) {
		case SINGLE_TAP: register_code(KC_LSFT); register_code(KC_0); break;
		case DOUBLE_TAP: register_code(KC_RBRC); break;
		case TRIPLE_TAP: register_code(KC_LSFT); register_code(KC_RBRC); break;
	}
}

void rbrk_reset(qk_tap_dance_state_t *state, void *user_data) {
	switch (xtap_state.state) {
		case SINGLE_TAP: unregister_code(KC_LSFT); unregister_code(KC_0); break;
		case DOUBLE_TAP: unregister_code(KC_RBRC); break;
		case TRIPLE_TAP: unregister_code(KC_LSFT); unregister_code(KC_RBRC); break;
	}
	xtap_state.state = 0;
}

qk_tap_dance_action_t tap_dance_actions[] = {
	[L_BRK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lbrk_finished, lbrk_reset),
	[R_BRK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rbrk_finished, rbrk_reset)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * 	,-----------------------------------------.					,-----------------------------------------.
 * 	| ESC  |  Q   |  W   |  E   |  R   |  T   | 					|  Y   |  U   |  I   |  O   |  P   | BSPC |
 * 	|------+------+------+------+------+------|					|------+------+------+------+------+------|
 * 	| TAB  |  A   |  S   |  D   |  F   |  G   |					|  H   |  J   |  K   |  L   | ;  : | ' "  |
 * 	|------+------+------+------+------+------+-------------.	  ,-------------+------+------+------+------+------+------|
 * 	| LSFT |  Z   |  X   |  C   |  V   |  B   | ([{  | DEL  |	  | GUI  | )]}  |  N   |  M   | ,  < | . >  | /  ? |SFTENT|
 *	`--------------------+------+------+------+------+------|	  |------+------+------+------+------+--------------------'
 *  	                     |  F13 | ALT  | CTL  | SPC  |      |	  |      | SPC  | CTL  | ALT  | PSCR |
 *  	                     |      |      |      | RAISE| LOWER|	  | LOWER| RAISE|      |      |      |
 *   	                     `----------------------------------'	  `----------------------------------'
 */
   [_QWERTY] = LAYOUT(
	KC_ESC,	KC_Q,	KC_W,	KC_E,	KC_R,	KC_T,							KC_Y,	KC_U,	KC_I,	KC_O,	KC_P,	KC_BSPC,
	KC_TAB,	KC_A,	KC_S,   KC_D,   KC_F,   KC_G,							KC_H,	KC_J,	KC_K,	KC_L,	KC_SCLN,KC_QUOT,
	KC_LSFT,KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,		TD(L_BRK),KC_DEL,KC_RGUI,TD(R_BRK),	KC_N,	KC_M,	KC_COMM,KC_DOT, KC_SLSH,KC_SFTENT,
		KC_F13, KC_LALT,KC_LCTL,	LT(_RAISE, KC_SPC), MO(_LOWER), MO(_LOWER), LT(_RAISE, KC_SPC),		KC_RCTL,KC_RALT,LCTL(KC_PSCR)
	),
/*
 * Raise Layer: Win Keys, F Keys, Numpad
 *
 * 	,-----------------------------------------.					,-----------------------------------------.
 * 	|      |      |      |      |      |      |					|  /   |  7   |  8   |  9   |  -   |      |
 * 	|------+------+------+------+------+------|					|------+------+------+------+------+------|
 * 	|      |  F1  |  F2  |  F3  |  F4  |  F5  |					|  *   |  4   |  5   |  6   |  +   |      |
 * 	|------+------+------+------+------+------+-------------.	  ,-------------+------+------+------+------+------+------|
 * 	| CAPS |  F6  |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |	  |      |LEAGUE|  0   |  1   |  2   |  3   |  .   |  =   |
 * 	`--------------------+------+------+------+------+------|	  |------+------+------+------+------+--------------------'
 *                           |      |      |      |  XX  |      |	  |      |  XX  |      |      |      |
 *                           `----------------------------------'	  `----------------------------------'
 */
   [_RAISE] = LAYOUT(
	_______,_______,_______,_______,_______,_______,						KC_SLSH,KC_7,	KC_8,	KC_9,	KC_MINS,_______,
	_______,KC_F1,	KC_F2,	KC_F3,	KC_F4,	KC_F5,							KC_ASTR,KC_4,	KC_5,	KC_6,	KC_PLUS,_______,
	KC_CAPS,KC_F6,	KC_F7,	KC_F8,	KC_F9,	KC_F10,		KC_F11,	KC_F12,	_______,TG(_LEAGUE),	KC_0,	KC_1,	KC_2,	KC_3,	KC_DOT,	KC_EQL,
				_______,_______,_______,	_______,_______,_______,_______,	_______,_______,_______
	),
/*
 * Lower Layer: Symbols, Arrows, Media, Navigation
 *
 * 	,-----------------------------------------.					,-----------------------------------------.
 * 	|  `   |  !   |  @   |  #   |  $   |  %   |					|  ^   |  &   |  *   |  _   | \  | |      |
 * 	|------+------+------+------+------+------|					|------+------+------+------+------+------|
 * 	|  ~   |      | PREV | PLAY | NEXT | MUTE |					|      |  UP  |      | PGUP | HOME |      |
 * 	|------+------+------+------+------+------+-------------.	  ,-------------+------+------+------+------+------+------|
 * 	|      |      |      |      |      |      |      |      |	  | GAME |      | LEFT | DOWN | RGHT | PGDN | END  |      |
 * 	`--------------------+------+------+------+------+------|	  |------+------+------+------+------+--------------------'
 *     	                     |      |      |      |      |  XX  |	  |  XX  |      |      |      |      |
 *        	             `----------------------------------'	  `----------------------------------'
 */
   [_LOWER] = LAYOUT(
	KC_GRV,	KC_EXLM,KC_AT,	KC_HASH,KC_DLR,	KC_PERC,						KC_CIRC,KC_AMPR,KC_ASTR,KC_UNDS,KC_BSLS,_______,
	KC_TILD,_______,KC_MPRV,KC_MPLY,KC_MNXT,KC_MUTE,						_______,KC_UP,	_______,KC_PGUP,KC_HOME,_______,
	_______,_______,_______,_______,_______,_______,	_______,_______,TG(_GAME),_______,	KC_LEFT,KC_DOWN,KC_RGHT,KC_PGDN,KC_END,	_______,
      				_______,_______,_______,	_______,_______,_______,_______,	_______,_______,_______
	),
/*
 * Adjust Layer: Function keys, RGB, Power
 *
 * 	,-----------------------------------------.					,-----------------------------------------.
 * 	|      |      | SAI  | HUI  | VAI  | MOD  |					|      |      |      |      |      |      |
 * 	|------+------+------+------+------+------|					|------+------+------+------+------+------|
 * 	|      | TOG  | SAD  | HUD  | VAD  | RMOD |					|      |      |      |      |      |      |
 * 	|------+------+------+------+------+------+-------------.	  ,-------------+------+------+------+------+------+------|
 * 	|      |      |      |      |      |      |      |      |	  |      |      |      |      |      |      |      |      |
 * 	`--------------------+------+------+------+------+------|	  |------+------+------+------+------+--------------------'
 *                           | SHUT |      | SLEEP|  XX  |  XX  |	  |  XX  |  XX  | LOCK |      | SHUT |
 *                           `----------------------------------'	  `----------------------------------'
 */
   [_ADJUST] = LAYOUT(
	_______,_______,RGB_SAI,RGB_HUI,RGB_VAI,RGB_MOD,						_______,_______,_______,_______,_______,_______,
	_______,RGB_TOG,RGB_SAD,RGB_HUD,RGB_VAD,RGB_RMOD,						_______,_______,_______,_______,_______,_______,
	_______,_______,_______,_______,_______,_______,	_______,_______,_______,_______,	_______,_______,_______,_______,_______,_______,
				KC_PWR,	_______,KC_SLEP,	_______,_______,_______,_______,	LGUI(KC_L),_______,KC_PWR
	),
/* 
 * Game Layer: Shifted WASD, 345, Removed Left R/L
 *
 * 	,-----------------------------------------.					,-----------------------------------------.
 *	|  T   | ESC  |  Q   |  W   |  E   |  R   |					|      |      |      |      |      |      |
 * 	|------+------+------+------+------+------|					|------+------+------+------+------+------|
 * 	|  G   | TAB  |  A   |  S   |  D   |  F   |					|      |      |      |      |      |      |
 * 	|------+------+------+------+------+------+-------------.	  ,-------------+------+------+------+------+------+------|
 * 	|  B   | SFT  |  Z   |  X   |  C   |  V   |  3   |  4   |	  |      |      |      |      |      |      |      |      |
 * 	`--------------------+------+------+------+------+------|	  |------+------+------+------+------+--------------------'
 * 	                     |      |      |      | SPC  |  5   |	  |      |      |      |      |      |
 *			     `----------------------------------'	  `----------------------------------'
 */
   [_GAME] = LAYOUT(
	KC_T,	KC_ESC,	KC_Q,	KC_W,	KC_E,	KC_R,							_______,_______,_______,_______,_______,_______,
	KC_G,	KC_TAB,	KC_A,	KC_S,	KC_D,	KC_F,							_______,_______,_______,_______,_______,_______,
	KC_B,	KC_LSFT,KC_Z,	KC_X,	KC_C,	KC_V,		KC_3,	KC_4,	_______,_______,	_______,_______,_______,_______,_______,_______,
				_______,_______,_______,	KC_SPC,	KC_5,	_______,_______,	_______,_______,_______
	),
/* 
 * League Layer: Shifted QWERTY, 12345, OPB left hand, Removed Left R/L
 *
 * 	,-----------------------------------------.					,-----------------------------------------.
 * 	|      |  1   |  2   |  3   |  4   |  5   |					|      |      |      |      |      |      |
 * 	|------+------+------+------+------+------|					|------+------+------+------+------+------|
 * 	|      |  Q   |  W   |  E   |  R   |  T   |					|      |      |      |      |      |      |
 * 	|------+------+------+------+------+------+-------------.	  ,-------------+------+------+------+------+------+------|
 * 	|      |  A   |  S   |  D   |  F   |  G   |  O   |  P   |	  |      |      |      |      |      |      |      |      |
 * 	`--------------------+------+------+------+------+------|	  |------+------+------+------+------+--------------------'
 *                           |      |      |      | SPC  |  B   |	  |      |      |      |      |      |
 *                           `----------------------------------'	  `----------------------------------'
 */
   [_LEAGUE] = LAYOUT(
	_______,KC_1,	KC_2,	KC_3,	KC_4,	KC_5,							_______,_______,_______,_______,_______,_______,
	_______,KC_Q,	KC_W,	KC_E,	KC_R,	KC_T,							_______,_______,_______,_______,_______,_______,
	_______,KC_A,	KC_S,	KC_D,	KC_F,	KC_G,		KC_O,	KC_P,	_______,_______,	_______,_______,_______,_______,_______,_______,
				_______,_______,_______,	KC_SPC,	KC_B,	_______,_______,	_______,_______,_______
	),

// 	/*
//  	* Layer template
//  	*
//  	* 	,-----------------------------------------.					,-----------------------------------------.
//  	* 	|      |      |      |      |      |      |					|      |      |      |      |      |      |
//  	* 	|------+------+------+------+------+------|					|------+------+------+------+------+------|
//  	* 	|      |      |      |      |      |      |					|      |      |      |      |      |      |
//  	* 	|------+------+------+------+------+------+-------------.	  ,-------------+------+------+------+------+------+------|
//  	* 	|      |      |      |      |      |      |      |      |	  |      |      |      |      |      |      |      |      |
//  	* 	`--------------------+------+------+------+------+------|	  |------+------+------+------+------+--------------------'
//  	*                            |      |      |      |      |      |	  |      |      |      |      |      |
//  	*                            `----------------------------------'	  `----------------------------------'
//  	*/
//     	    [_LAYERINDEX] = LAYOUT(
// 		_______,_______,_______,_______,_______,_______,						_______,_______,_______,_______,_______,_______,
//       	_______,_______,_______,_______,_______,_______,						_______,_______,_______,_______,_______,_______,
//       	_______,_______,_______,_______,_______,_______,	_______,_______,_______,_______,	_______,_______,_______,_______,_______,_______,
//					_______,_______,_______,	_______,_______,_______,_______,	_______,_______,_______
//      	),
};

layer_state_t layer_state_set_user(layer_state_t state) {
	state = update_tri_layer_state (state, _LOWER, _RAISE, _ADJUST);
	switch (get_highest_layer(state)) {
		case _RAISE:
			rgblight_enable();
			rgblight_sethsv_noeeprom (HSV_ORANGE);
			break;
		case _LOWER:
			rgblight_enable();
			rgblight_sethsv_noeeprom (HSV_GREEN);
			break;
		case _ADJUST:
			rgblight_enable();
			rgblight_sethsv_noeeprom (HSV_PURPLE);
			break;
		case _GAME:
			rgblight_enable();
			rgblight_sethsv_noeeprom (HSV_RED);
			break;
		case _LEAGUE:
			rgblight_enable();
			rgblight_sethsv_noeeprom (HSV_BLUE);
			break;
		default:
			rgblight_disable();
			break;
		}
	return state;
}

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
	if (index == 0) {
		// Volume control
		if (clockwise) {
			tap_code(KC_VOLD);
		} else {
			tap_code(KC_VOLU);
		}
	}
	else if (index == 1) {
		// Scroll by word
		if (clockwise) {
			tap_code16(C(KC_LEFT));
		} else {
			tap_code16(C(KC_RGHT));
		}
	}
}
#endif
