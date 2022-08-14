#include QMK_KEYBOARD_H
// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number {
  _BASE = 0,
  _L1,
  _L2,
  _L3,
};

// 以下追加
static bool gs_lshift = false;
static bool gs_rshift = false;

void proc_regist_keycode(keyrecord_t *record, uint16_t regist_keycode_ifshift, bool is_shift_ifshift, uint16_t regist_keycode, bool is_shift){
    bool shift_now = gs_lshift || gs_rshift;

    if (record->event.pressed) {
        //キーを押した場合

        if(shift_now){
            //今シフトが押されている場合

            if(!is_shift_ifshift){
                //変換先キーがシフト無しの場合、シフトを解除する
                if (gs_lshift) unregister_code(KC_LSFT);
                if (gs_rshift) unregister_code(KC_RSFT);
            }

            //変換先キー押下
            register_code(regist_keycode_ifshift);
        }else{
            //今シフトが押されていない場合

            //変換先キーがシフトありの場合、シフトを押下する
            if(is_shift)register_code(KC_LSFT);

            //変換先キー押下
            register_code(regist_keycode);
        }
    }else{
        //キーを放した場合
        //先にシフト状態が変わっている場合があるため、両方の変換先を放す

        //----シフト側

        if(shift_now && !is_shift_ifshift){
            //変換先キーがシフト無しの場合、シフトを解除する
            if (gs_lshift) unregister_code(KC_LSFT);
            if (gs_rshift) unregister_code(KC_RSFT);
        }

        //変換先キーを放す
        unregister_code(regist_keycode_ifshift);

        if(shift_now && !is_shift_ifshift){
            //シフトの状態を戻す
            if (gs_lshift) register_code(KC_LSFT);
            if (gs_rshift) register_code(KC_RSFT);
        }

        //----シフトしない側

        //変換先キーがシフトありの場合、シフトを押下する
        if(!shift_now && is_shift)register_code(KC_LSFT);

        //変換先キーを放す
        unregister_code(regist_keycode);

        //シフトの状態を戻す
        if(!shift_now && is_shift)unregister_code(KC_LSFT);
    }
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    //シフトが押下または放された場合は、その状態を保持して終了
    switch (keycode) {
        case KC_LSFT:
            if (record->event.pressed) gs_lshift = true;
            else gs_lshift = false;
            return true;
        case KC_RSFT:
            if (record->event.pressed) gs_rshift = true;
            else gs_rshift = false;
            return true;
    }

    //実際のシフト押下と今のレジスト状態が異なる場合は実際の押下に合わせる
    bool is_reg_lshift = keyboard_report->mods & MOD_BIT(KC_LSFT);
    bool is_reg_rshift = keyboard_report->mods & MOD_BIT(KC_RSFT);
    if(!is_reg_lshift && gs_lshift)register_code(KC_LSFT);
    if(is_reg_lshift && !gs_lshift)unregister_code(KC_LSFT);
    if(!is_reg_rshift && gs_rshift)register_code(KC_RSFT);
    if(is_reg_rshift && !gs_rshift)unregister_code(KC_RSFT);

    //JISと異なるキーそれぞれの分岐
    //２，３番目の引数がシフトが押下されている場合の変換先
    //４，５番目の引数がシフト無しの場合の変換先
    //３，５番目の引数は変換先がシフトを必要とするかどうか　true:必要　false:不要
    switch (keycode) {
        case KC_2:
            proc_regist_keycode(record, KC_LBRC, false, KC_2, false);
            return false;
        case KC_6:
            proc_regist_keycode(record, KC_EQL, false, KC_6, false);
            return false;
        case KC_7:
            proc_regist_keycode(record, KC_6, true, KC_7, false);
            return false;
        case KC_8:
            proc_regist_keycode(record, KC_QUOT, true, KC_8, false);
            return false;
        case KC_9:
            proc_regist_keycode(record, KC_8, true, KC_9, false);
            return false;
        case KC_0:
            proc_regist_keycode(record, KC_9, true, KC_0, false);
            return false;
        case KC_MINS:
            proc_regist_keycode(record, KC_INT1, true, KC_MINS, false);
            return false;
        case KC_EQL:
            proc_regist_keycode(record, KC_SCLN, true, KC_MINS, true);
            return false;
        case KC_LBRC:
            proc_regist_keycode(record, KC_RBRC, true, KC_RBRC, false);
            return false;
        case KC_RBRC:
            proc_regist_keycode(record, KC_NUHS, true, KC_NUHS, false);
            return false;
        case KC_BSLS:
            proc_regist_keycode(record, KC_INT3, true, KC_INT1, false);
            return false;
        case KC_SCLN:
            proc_regist_keycode(record, KC_QUOT, false, KC_SCLN, false);
            return false;
        case KC_QUOT:
            proc_regist_keycode(record, KC_2, true, KC_7, true);
            return false;
        case KC_GRV:
            proc_regist_keycode(record, KC_EQL, true, KC_LBRC, true);
            return false;
        case KC_CAPS:
            proc_regist_keycode(record, KC_CAPS, true, KC_CAPS, true);
            return false;
        case KC_TILD:
            proc_regist_keycode(record, KC_EQL, true, KC_EQL, true);
            return false;
        case KC_AT:
            proc_regist_keycode(record, KC_LBRC, false, KC_LBRC, false);
            return false;
        case KC_CIRC:
            proc_regist_keycode(record, KC_EQL, false, KC_EQL, false);
            return false;
        case KC_AMPR:
            proc_regist_keycode(record, KC_6, true, KC_6, true);
            return false;
        case KC_ASTR:
            proc_regist_keycode(record, KC_QUOT, true, KC_QUOT, true);
            return false;
        case KC_LPRN:
            proc_regist_keycode(record, KC_8, true, KC_8, true);
            return false;
        case KC_RPRN:
            proc_regist_keycode(record, KC_9, true, KC_9, true);
            return false;
        case KC_UNDS:
            proc_regist_keycode(record, KC_INT1, true, KC_INT1, true);
            return false;
        case KC_PLUS:
            proc_regist_keycode(record, KC_SCLN, true, KC_SCLN, true);
            return false;
        case KC_LCBR:
            proc_regist_keycode(record, KC_RBRC, true, KC_RBRC, true);
            return false;
        case KC_RCBR:
            proc_regist_keycode(record, KC_NUHS, true, KC_NUHS, true);
            return false;
        case KC_PIPE:
            proc_regist_keycode(record, KC_INT3, true, KC_INT3, true);
            return false;
        case KC_COLN:
            proc_regist_keycode(record, KC_QUOT, false, KC_QUOT, false);
            return false;
        case KC_DQUO:
            proc_regist_keycode(record, KC_2, true, KC_2, true);
            return false;
    }
    return true;
}
// 追加ここまで

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT(
  //,-----------------------------------------------------|   |--------------------------------------------------------------------------------.
       KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,        KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,  KC_EQL, KC_BSLS,  KC_GRV,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_LBRC, KC_RBRC,KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------|
     KC_LCTRL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,  KC_ENT,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT, MO(_L1),
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------|
               KC_LALT, KC_LGUI,  KC_SPC,  KC_SPC,               KC_SPC,  KC_SPC,          KC_RGUI, KC_RALT
          //`---------------------------------------------|   |--------------------------------------------'
  ),

  [_L1] = LAYOUT(
  //,-----------------------------------------------------|   |--------------------------------------------------------------------------------.
  TG(_L2),   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,       KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,  KC_INS,  KC_DEL,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,     _______, _______, KC_PSCR, KC_SLCK,KC_PAUSE,   KC_UP, _______, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,     _______, _______, KC_HOME, KC_PGUP, KC_LEFT,KC_RIGHT, _______,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,     _______, _______,  KC_END, KC_PGDN, KC_DOWN, _______, _______,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------|
               _______, _______, _______, _______,              _______, _______,          KC_STOP, _______
          //`---------------------------------------------|   |--------------------------------------------'
  ),

  [_L2] = LAYOUT( /* Base */
  //,-----------------------------------------------------|   |--------------------------------------------------------------------------------.
 TG(_L2), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   QK_BOOT,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     RGB_TOG, RGB_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     RGB_VAD, RGB_VAI, RGB_HUD, RGB_HUI, RGB_SAD, RGB_SAI, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------|
               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,              XXXXXXX, XXXXXXX,          KC_STOP, XXXXXXX
          //`---------------------------------------------|   |--------------------------------------------'
  ),

  [_L3] = LAYOUT( /* Base */
  //,-----------------------------------------------------|   |--------------------------------------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   QK_BOOT,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------|
               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,              XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX
          //`---------------------------------------------|   |--------------------------------------------'
  )
};


//A description for expressing the layer position in LED mode.
layer_state_t layer_state_set_user(layer_state_t state) {
#ifdef RGBLIGHT_ENABLE
    switch (get_highest_layer(state)) {
    case _L1:
      rgblight_sethsv_at(HSV_BLUE, 0);
      break;
    case _L2:
      rgblight_sethsv_at(HSV_RED, 0);
      break;
    case _L3:
      rgblight_sethsv_at(HSV_PURPLE, 0);
      break;
    default: //  for any other layers, or the default layer
      rgblight_sethsv_at( 0, 0, 0, 0);
      break;
    }
    rgblight_set_effect_range( 1, 11);
#endif
return state;
}
