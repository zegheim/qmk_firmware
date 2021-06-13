#pragma once

bool is_alt_tab_active = false;
uint8_t mod_state;
uint16_t alt_tab_timer = 0;

enum custom_keycodes {
    BSP_DEL = SAFE_RANGE,
};

void matrix_scan_user(void) {
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 1000) {
            unregister_code(KC_LALT);
            is_alt_tab_active = false;
        }
    }
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    // enables passing key sequences to the host system for a few seconds.
    tap_code16(C(KC_F2));

    alt_tab_timer = timer_read();

    if (!is_alt_tab_active) {
        is_alt_tab_active = true;
        register_code(KC_LALT);
    }

    if (clockwise) {
        tap_code(KC_TAB);
    } else {
        tap_code16(S(KC_TAB));
    }

    return true;
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    mod_state = get_mods();
    rgb_matrix_mode(RGB_MATRIX_MULTISPLASH);
    switch (keycode) {
        case BSP_DEL:
            if (record->event.pressed) {
                if (mod_state & MOD_BIT(KC_LSFT)) {
                    del_mods(MOD_BIT(KC_LSFT)); // Cancel l-shift so it isn't applied to KC_DEL 
                    register_code(KC_DEL);
                    set_mods(mod_state); // Reapply cancelled l-shift so it continues to work after KC_DEL
                } else {
                    register_code(KC_BSPC);
                }
            } else {
                unregister_code(KC_DEL);
                unregister_code(KC_BSPC);
            }
            return false;
        default:
            return true;
    }
};