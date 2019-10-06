#include QMK_KEYBOARD_H

extern keymap_config_t keymap_config;

enum custom_keycodes {
  DBLSLAS = SAFE_RANGE,
  PWRSHLL,
  SHT_DWN,
};

static bool shift_held = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  static uint16_t sht_dwn_timer;
  static uint8_t saved_mods;

  switch(keycode) {
    case PWRSHLL:
      if (record->event.pressed) {
        tap_code16(G(KC_X));
        tap_code(KC_A);
      }
      return false;

    case SHT_DWN:
      if(record->event.pressed) {
        sht_dwn_timer = timer_read();
        if (timer_elapsed(sht_dwn_timer) < TAPPING_TERM) {
          tap_code(KC_SLEP);
        } else {
          tap_code(KC_PWR);
        }
      }
      return false;

    case DBLSLAS:
      if (record->event.pressed) {
        if (shift_held) {
          saved_mods = get_mods() & MOD_MASK_SHIFT;
          del_mods(MOD_MASK_SHIFT);
          register_code(KC_BSLS);
        } else {
          register_code(KC_SLSH);
        }
      } else {
        unregister_code(KC_SLSH);
        unregister_code(KC_BSLS);
        add_mods(saved_mods);
      }
      return false;      

    case KC_LSFT:
      shift_held = record->event.pressed;
      return true;

    case KC_RSFT:
      shift_held = record->event.pressed;
      return true;
  }
  return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* Base
     * .-----------------------------------.      .-----------------------------------.
     * | ESC |  1  |  2  |  3  |  4  |  5  |      |  6  |  7  |  8  |  9  |  0  |/and\|
     * |-----+-----+-----+-----+-----+-----|      |-----+-----+-----+-----+-----+-----|
     * | TAB |  Q  |  W  |  E  |  R  |  T  |      |  Y  |  U  |  I  |  O  |  P  | BSP |
     * |-----+-----+-----+-----+-----+-----|      |-----+-----+-----+-----+-----+-----|
     * |CAPLK|  A  |  S  |  D  |  F  |  G  |      |  H  |  J  |  K  |  L  |  ;  |ENTER|
     * |-----+-----+-----+-----+-----+-----|      |-----+-----+-----+-----+-----+-----|
     * |SHIFT|  Z  |  X  |  C  |  V  |  B  |      |  N  |  M  |  ,  |  .  |  '  |SHIFT|
     * |-----+-----+-----+-----+-----+-----|      |-----+-----+-----+-----+-----+-----|
     * | CTR | GUI | ALT |RAISE|   SPACE   |      |   SPACE   |  <  |  ^  |  v  |  >  |
     * '-----------------------------------'      '-----------------------------------'
     */

    [0] = LAYOUT(
        KC_ESC,  KC_1,    KC_2,    KC_3,  KC_4,   KC_5,   KC_6,   KC_7,   KC_8,    KC_9,   KC_0,    DBLSLAS, \
        KC_TAB,  KC_Q,    KC_W,    KC_E,  KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,    KC_O,   KC_P,    KC_BSPC, \
        KC_CAPS, KC_A,    KC_S,    KC_D,  KC_F,   KC_G,   KC_H,   KC_J,   KC_K,    KC_L,   KC_SCLN, KC_ENT,  \
        KC_LSFT, KC_Z,    KC_X,    KC_C,  KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT, KC_QUOT, KC_RSFT, \
        KC_LCTL, KC_LGUI, KC_LALT, MO(1), KC_SPC, KC_SPC, KC_SPC, KC_SPC, KC_LEFT, KC_UP,  KC_DOWN, KC_RIGHT \
    ),

    /* Functions
     * .-----------------------------------.      .-----------------------------------.
     * |POWER| F01 | F02 | F03 | F04 | F05 |      | F06 | F07 | F08 | F09 | F10 |C-A-D|
     * |-----+-----+-----+-----+-----+-----|      |-----+-----+-----+-----+-----+-----|
     * |LITE+| F11 | F12 | F13 | F14 | F15 |      |  ~  |  ?  |  |  |  [  |  ]  | DEL |
     * |-----+-----+-----+-----+-----+-----|      |-----+-----+-----+-----+-----+-----|
     * |LITE-| F16 | F17 | F18 | F19 | F20 |      |  +  |  -  |  `  |  {  |  }  |SHELL|
     * |-----+-----+-----+-----+-----+-----|      |-----+-----+-----+-----+-----+-----|
     * |CLEAR| F21 | F22 | F23 | F24 |BLANK|      |  =  |  _  | < < | > > | ||> | =(x |
     * |-----+-----+-----+-----+-----+-----|      |-----+-----+-----+-----+-----+-----|
     * |CLEAR|CLEAR|CLEAR|CLEAR|   BLANK   |      |   BLANK   |BLANK|BLANK|BLANK|BLANK|
     * '-----------------------------------'      '-----------------------------------'
     */

    [1] = LAYOUT(
        SHT_DWN, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  C(A(KC_DEL)), \
        BL_INC,  KC_F11,  KC_F12,  KC_F13,  KC_F14,  KC_F15,  KC_TILD, KC_QUES, KC_PIPE, KC_LBRC, KC_RBRC, KC_DEL,       \
        BL_DEC,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_PLUS, KC_MINS, KC_GRV,  KC_LCBR, KC_RCBR, PWRSHLL,      \
        _______, KC_F21,  KC_F22,  KC_F23,  KC_F24,  XXXXXXX, KC_EQL,  KC_UNDS, KC_MPRV, KC_MNXT, KC_MPLY, _______,      \
        _______, _______, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLD, KC_VOLU, KC_MUTE, XXXXXXX       \
    )
};
