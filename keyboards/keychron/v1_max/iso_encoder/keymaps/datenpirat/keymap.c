#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "keychron_wireless_common.h"

#define PERMISSIVE_HOLD_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define RETRO_TAPPING_PER_KEY


enum {
    L_VANILLA,
    L_BASE,
    L_FN,
    L_CAPS,
    L_CAPS_2,
    L_RCTL,
    L_NUBS,
    L_NUBS_2
};

typedef struct {
  bool is_press_action;
  int state;
} tap;

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4,
  DOUBLE_SINGLE_TAP = 5, //send two single taps
  TRIPLE_TAP = 6,
  TRIPLE_HOLD = 7,
  SINGLE_HOLD_INTERRUPTED = 8,
  TAP_END = 9
};

int get_dance_state (tap_dance_state_t *state) {
  if (state->count == 1) {
      if (state->interrupted && state->pressed)  return SINGLE_HOLD_INTERRUPTED;    // taste wurde während des gedrückthaltens unterbrochen
      else if (state->interrupted || !state->pressed)  return SINGLE_TAP;
      //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
      else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    /*
     * DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
     * action when hitting 'pp'. Suggested use case for this return value is when you want to send two
     * keystrokes of the key, and not the 'double tap' action/macro.
    */
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  //Assumes no one is trying to type the same letter three times (at least not quickly).
  //If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
  //an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
  if (state->count == 3) {
    if (state->interrupted || !state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return TAP_END; //magic number. At some point this method will expand to work for more presses
}


//instanalize an instance of 'tap' for the 'x' tap dance.
static tap tap_state = {
  .is_press_action = true,
  .state = 0
};

// Tap Dance declarations
enum {
    TD_CAPS,
};

// Tap Dance functions CAPS
void super_CAPS_start (tap_dance_state_t *state, void *user_data) {
  if (state->count == 1){
      layer_on(L_CAPS);
  }
}

void super_CAPS_finished (tap_dance_state_t *state, void *user_data) {
  tap_state.state = get_dance_state(state);
  switch (tap_state.state) {
    case SINGLE_TAP:
        tap_code(KC_HOME);
        break;
    case SINGLE_HOLD:
        //layer_on(FN_LAYER_2);_______
        break;
    case DOUBLE_TAP:
        register_code(KC_CAPS);
        break;
    case DOUBLE_HOLD:
        break;
    case DOUBLE_SINGLE_TAP:
        break;
    case TRIPLE_TAP:
        break;
  }
}
void super_CAPS_reset (tap_dance_state_t *state, void *user_data) {

    switch (tap_state.state) {
        case SINGLE_TAP:
            //caps_word_off();
            break;
        case SINGLE_HOLD:
            break;
        case DOUBLE_TAP:
            unregister_code(KC_CAPS);
            break;
        case DOUBLE_HOLD:
            break;
        case DOUBLE_SINGLE_TAP:
            break;
        case TRIPLE_TAP:
            break;
        default:
            break;
    }

    layer_off(L_CAPS);
    tap_state.state = 0;
}

//Tap Dance Definitions
tap_dance_action_t tap_dance_actions[] = {
    [TD_CAPS]        = ACTION_TAP_DANCE_FN_ADVANCED(super_CAPS_start ,super_CAPS_finished, super_CAPS_reset),
};


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [L_VANILLA] = LAYOUT_iso_83(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_INS,             KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,                      KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,             KC_END,
        KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT, MO(L_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
    ),

    [L_BASE] = LAYOUT_iso_83(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_INS,             KC_MPLY,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,                      KC_PGDN,
     TD(TD_CAPS), KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,             KC_END,
        KC_LSFT,  LT(L_NUBS,KC_NUBS), KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT  ,  KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                         KC_SPC,                              KC_RALT, LT(L_FN,KC_APP), LT(L_RCTL, KC_RCTL),  KC_LEFT,  KC_DOWN,  KC_RGHT
    ),


    [L_FN] = LAYOUT_iso_83(
        _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_DEL ,            RGB_TOG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  KC_PSCR,  _______,  _______,                      _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  KC_LEFT,  KC_DOWN,  KC_UP  ,  KC_RGHT,  _______,  _______,  _______,            KC_HOME,
        _______,  KC_NUBS,  _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,  _______,
        _______,  _______,  _______,                                KC_MPLY,                                _______,  _______,  _______,  _______,  _______,  _______
    ),


    [L_CAPS] = LAYOUT_iso_83(
        XXXXXXX ,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,           KC_MPLY,
        XXXXXXX,  KC_P1,    KC_P2,    KC_P3,    KC_P4,    KC_P5,    KC_P6,    KC_P7,    KC_P8,    KC_P9,    KC_P0,    XXXXXXX,  XXXXXXX,  XXXXXXX,            KC_UP,
        XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_WH_U,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_PGUP,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                      KC_DOWN,
        _______,  KC_HOME,  KC_WH_L,  KC_WH_D,  KC_WH_R,  KC_HOME,  KC_LEFT,  KC_DOWN  ,KC_UP  ,  KC_RGHT,  KC_END ,  XXXXXXX,  XXXXXXX,  KC_PENT,            XXXXXXX,
        XXXXXXX,  KC_NUBS,  XXXXXXX,  XXXXXXX,  KC_LCTL,  KC_LSFT,  XXXXXXX,  XXXXXXX,  KC_PGDN,  XXXXXXX,  KC_PDOT,  XXXXXXX,            KC_HOME,  KC_PGUP,
        XXXXXXX,  XXXXXXX,  XXXXXXX,                               LT(L_CAPS_2,KC_MS_BTN1),                           XXXXXXX,  KC_APP ,  XXXXXXX,  KC_HOME,  KC_PGDN,  KC_END
    ),

    [L_CAPS_2] = LAYOUT_iso_83(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      _______,
        _______,  _______,  _______,  _______,  _______,  _______,  KC_HOME,  KC_PGDN,  KC_PGUP,  KC_END ,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______ , _______,  _______,  _______,  _______
    ),


    [L_NUBS] = LAYOUT_iso_83(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  KC_MS_UP,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      _______,
        _______,  _______,  KC_MS_LEFT,  KC_MS_DOWN,  KC_MS_RIGHT,  _______,  _______,  KC_MS_BTN1,  KC_MS_BTN3  ,KC_MS_BTN2,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
        _______,  _______,  _______,                              MO(L_NUBS_2),                                _______,  _______ , _______,  _______,  _______,  _______
    ),

    [L_NUBS_2] = LAYOUT_iso_83(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______  ,_______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______ , _______,  _______,  _______,  _______
    ),

    [L_RCTL] = LAYOUT_iso_83(
        KC_NUM,  KC_PSCR,  KC_SCRL,  KC_PAUS,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_MUTE,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______  ,_______,  _______,  _______,  _______,  KC_CALC,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
        _______,  _______,  _______,                                KC_MPLY,                                _______,  _______ , _______,  KC_MPRV,  _______,  KC_MNXT
    )

};

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        /*
        case LT(L_CAPS,KC_SPC):
            // Immediately select the hold action when another key is tapped.
            return true;
        */
        default:
            // Do not select the hold action when another key is tapped.
            return false;
    }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        //case LT(L_CAPS,KC_SPC):
            // Immediately select the hold action when another key is pressed.
        //    return true;
        default:
            // Do not select the hold action when another key is pressed.
            return false;
    }
}
bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(L_CAPS,KC_SPC): // Press space if Space but no other key was pressed.
            return true;
        default:
            return false;
    }
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {

        /* First encoder */
        if(IS_LAYER_ON(L_CAPS_2)){
            if (clockwise)
                tap_code(KC_WH_R);
             else
                tap_code(KC_WH_L);
        }

        else if(IS_LAYER_ON(L_CAPS)){
            if (clockwise)
                tap_code(KC_WH_D);
             else
                tap_code(KC_WH_U);
        }

        else if(IS_LAYER_ON(L_NUBS_2)){
            if (clockwise)
                tap_code(KC_DOWN);
            else
                tap_code(KC_UP);
        }


        else if(IS_LAYER_ON(L_NUBS)){
            if (clockwise)
                tap_code(KC_RGHT);
            else
                tap_code(KC_LEFT);
        }

        else if(IS_LAYER_ON(L_RCTL)){
            if (clockwise)
                tap_code(KC_MNXT);
            else
                tap_code(KC_MPRV);
        }


        else if(IS_LAYER_ON(L_FN)){
            if (clockwise)
                rgb_matrix_increase_val();
            else
                rgb_matrix_decrease_val();
        }


        else{ // BASE or VANILLA Layer

            // CTRL

            /*if (get_mods() & MOD_BIT(KC_LCTL)) {
                if (clockwise)
                    tap_code(KC_WH_R);
                else
                    tap_code(KC_WH_L);
            }
            */

            // ALT
            if (get_mods() & MOD_BIT(KC_LALT)) {
                if (clockwise)
                    tap_code(KC_TAB);
                else
                    tap_code16(LSFT(KC_TAB));
            }
            // no modifier
            else {
                if (clockwise)
                    tap_code(KC_VOLU);
                 else
                    tap_code(KC_VOLD);

            }

        }


    }

    return false; // do not run code after this
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_keychron_common(keycode, record)) {
        return false;
    }

    switch (keycode) {
        case KC_BSPC: {
            static uint16_t registered_key = KC_NO;

            if (record->event.pressed) {  // On key press.
                const uint8_t mods = get_mods();

                uint8_t shift_mods = (mods | get_oneshot_mods()) & MOD_MASK_SHIFT;

                if (shift_mods) {  // At least one shift key is held.

                    registered_key = KC_DEL;

                    // If one shift is held, clear it from the mods. But if both
                    // shifts are held, leave as is to send Shift + Del.
                    if (shift_mods != MOD_MASK_SHIFT) {
                        del_oneshot_mods(MOD_MASK_SHIFT);
                        unregister_mods(MOD_MASK_SHIFT);
                    }

                } else {
                    registered_key = KC_BSPC;
                }

                register_code(registered_key);
                set_mods(mods);
            }
            else {  // On key release.
                unregister_code(registered_key);
            }
        }
        return false;

        // Other macros...
    }

  return true;
}
