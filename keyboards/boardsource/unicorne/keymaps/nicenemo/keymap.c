#include QMK_KEYBOARD_H
#include <stdio.h> // Explicitly include for strict snprintf bounds checking
#include <string.h>

// ==========================================================================
// 1. Centralized Modifier String Definitions (Allocated Once)
// ==========================================================================
#define STR_NO_MODS         "NO MODS"
#define STR_L_HYPER         "L-HYPER"
#define STR_L_MEH           "L-MEH"
#define STR_BOTH_SHIFTS     "BOTH SHIFTS"
#define STR_BOTH_CONTROLS   "BOTH CONTROLS"
#define STR_BOTH_ALTS       "BOTH ALTS"
#define STR_BOTH_GUIS       "BOTH GUIS"
#define STR_L_SHIFT         "L-SHIFT"
#define STR_R_SHIFT         "R-SHIFT"
#define STR_L_CTRL          "L-CTRL"
#define STR_R_CTRL          "R-CTRL"
#define STR_L_ALT           "L-ALT"
#define STR_R_ALT           "R-ALT"
#define STR_L_GUI           "L-GUI"
#define STR_R_GUI           "R-GUI"

// ==========================================================================
// 2. Centralized OLED Layer Name Definitions (Allocated Once)
// ==========================================================================
#define STR_LNAME_BASE       "BASE"
#define STR_LNAME_ALT_BASE   "ALT-BASE"
#define STR_LNAME_RGB_HW     "RGB-HW"
#define STR_LNAME_SYM_CHRD   "SYM-CHRD"
#define STR_LNAME_BRACKETS   "BRACKETS"
#define STR_LNAME_NUMPAD     "NUMPAD"
#define STR_LNAME_NAV_EDIT   "NAV-EDIT"
#define STR_LNAME_FN_LOW     "FN-LOW"
#define STR_LNAME_FN_HIGH    "FN-HIGH"
#define STR_LNAME_MEDIA      "MEDIA"
#define STR_LNAME_MOUSE      "MOUSE"
#define STR_LNAME_MEH_LOW    "MEH-LOW"
#define STR_LNAME_MEH_HIGH   "MEH-HIGH"
#define STR_LNAME_HYPR_LOW   "HYPR-LOW"
#define STR_LNAME_HYPR_HI    "HYPR-HI"
#define STR_LNAME_SYSTEM     "SYSTEM"

// ==========================================================================
// 3. Color Defs: Baseline Layers & Explicit 8-Color Spectrum Defines
// ==========================================================================
#define RGB_RUBY_RED        255,   0,   0  // Layer 0  (BASE)
#define RGB_ELECTRIC_LIME     0, 255,   0  // Layer 1  (ALT-BASE)
#define RGB_TRUE_BLUE         0,   0, 255  // Layer 2  (RGB-HW)
#define RGB_CYBER_YELLOW    255, 255,   0  // Layer 3  (SYM-CHRD)
#define RGB_DEEP_MAGENTA    255,   0, 255  // Layer 4  (BRACKETS)
#define RGB_HYPER_CYAN        0, 255, 255  // Layer 5  (NUMPAD)
#define RGB_SAFETY_ORANGE   255,  85,   0  // Layer 6  (NAV-EDIT)
#define RGB_TOXIC_GREEN      85, 255,   0  // Layer 7  (FN-LOW)
#define RGB_NEON_PURPLE     119,   0, 255  // Layer 8  (FN-HIGH)
#define RGB_HOT_PINK        255,   0, 127  // Layer 9  (MEDIA)
#define RGB_MINT_ICE          0, 255, 127  // Layer 10 (MOUSE)
#define RGB_SKY_BLUE          0, 127, 255  // Layer 11 (MEH-LOW)
#define RGB_PALE_GOLD       255, 170,   0  // Layer 12 (MEH-HIGH)
#define RGB_WILD_ORCHID     170,   0, 255  // Layer 13 (HYPR-LOW)
#define RGB_PURE_WHITE      255, 255, 255  // Layer 14 (HYPR-HI)
#define RGB_WARM_CORAL      255,  85,  85  // Layer 15 (SYSTEM)

// Dedicated 8-Spectrum Color Definitions for Modifiers
#define RGB_BLAZE_ORANGE    255,  60,   0  // Left Ctrl  (LED 0)
#define RGB_HOT_MAGENTA     255,   0, 128  // Left Shift (LED 1)
#define RGB_NEON_LIME         0, 255,  60  // Left Alt   (LED 2)
#define RGB_ELECTRIC_BLUE     0, 160, 255  // Left GUI   (LED 3)
#define RGB_DEEP_VIOLET     170,   0, 255  // Right GUI  (LED 4)
#define RGB_CYBER_YELLOW_M  255, 230,   0  // Right Alt  (LED 5)
#define RGB_VIVID_CYAN        0, 255, 210  // Right Shift (LED 6)
#define RGB_CRIMSON_RED     255,   0,   0  // Right Ctrl (LED 7)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return true;
}

// ==========================================================================
// 4. Dynamic Text State Engine (Calculates OLED Text Strings Natively)
// ==========================================================================
static void evaluate_modifier_text(char *buffer, size_t buffer_size) {
    uint8_t mods = get_mods();
#ifdef ONE_SHOT_ENABLE
    mods |= get_oneshot_mods();
#endif

    // GUARD CLAUSE: Immediately exit if zero modifiers are active
    if (mods == 0) {
        snprintf(buffer, buffer_size, "%s", STR_NO_MODS);
        return;
    }

    // Direct Switch matching for distinct bitmask states using compliant QMK keycode tags
    switch (mods) {
        // --- Priority Aliases (Left Chords) ---
        case MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT) | MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI):
            snprintf(buffer, buffer_size, "%s", STR_L_HYPER); return;
        case MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT) | MOD_BIT(KC_LALT):
            snprintf(buffer, buffer_size, "%s", STR_L_MEH); return;

        // --- Mirror Matches ---
        case MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT): snprintf(buffer, buffer_size, "%s", STR_BOTH_SHIFTS); return;
        case MOD_BIT(KC_LCTL) | MOD_BIT(KC_RCTL): snprintf(buffer, buffer_size, "%s", STR_BOTH_CONTROLS); return;
        case MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT): snprintf(buffer, buffer_size, "%s", STR_BOTH_ALTS); return;
        case MOD_BIT(KC_LGUI) | MOD_BIT(KC_RGUI): snprintf(buffer, buffer_size, "%s", STR_BOTH_GUIS); return;

        // --- Monolithic Singles ---
        case MOD_BIT(KC_LSFT): snprintf(buffer, buffer_size, "%s", STR_L_SHIFT); return;
        case MOD_BIT(KC_RSFT): snprintf(buffer, buffer_size, "%s", STR_R_SHIFT); return;
        case MOD_BIT(KC_LCTL): snprintf(buffer, buffer_size, "%s", STR_L_CTRL);  return;
        case MOD_BIT(KC_RCTL): snprintf(buffer, buffer_size, "%s", STR_R_CTRL);  return;
        case MOD_BIT(KC_LALT): snprintf(buffer, buffer_size, "%s", STR_L_ALT);   return;
        case MOD_BIT(KC_RALT): snprintf(buffer, buffer_size, "%s", STR_R_ALT);   return;
        case MOD_BIT(KC_LGUI): snprintf(buffer, buffer_size, "%s", STR_L_GUI);   return;
        case MOD_BIT(KC_RGUI): snprintf(buffer, buffer_size, "%s", STR_R_GUI);   return;

        // --- Fallback Dynamic String Builder for arbitrary combinations ---
        default: {
            size_t offset = 0;
            buffer[0] = '\0';

            for (uint8_t i = 0; i < 8; i++) {
                if (mods & (1 << i)) {
                    const char *mod_str = "";
                    switch (1 << i) {
                        case MOD_BIT(KC_LCTL): mod_str = "LC "; break;
                        case MOD_BIT(KC_LSFT): mod_str = "LS "; break;
                        case MOD_BIT(KC_LALT): mod_str = "LA "; break;
                        case MOD_BIT(KC_LGUI): mod_str = "LG "; break;
                        case MOD_BIT(KC_RCTL): mod_str = "RC "; break;
                        case MOD_BIT(KC_RSFT): mod_str = "RS "; break;
                        case MOD_BIT(KC_RALT): mod_str = "RA "; break;
                        case MOD_BIT(KC_RGUI): mod_str = "RG "; break;
                    }

                    int written = snprintf(buffer + offset, buffer_size - offset, "%s", mod_str);

                    if (written > 0 && (size_t)written < buffer_size - offset) {
                        offset += written;
                    } else {
                        break;
                    }
                }
            }

            if (offset > 0 && buffer[offset - 1] == ' ') {
                buffer[offset - 1] = '\0';
            }
            break;
        }
    }
}

// ==========================================================================
// 5. Dual-Line OLED Screen Update Logic (Asymmetric Master/Slave Telemetry)
// ==========================================================================
#ifdef OLED_ENABLE
bool oled_task_user(void) {
    if (!is_keyboard_master()) {
        oled_write_ln(PSTR("UNICORNE SLAVE"), false);
        return false;
    }

    static const char *layer_names[] = {
        STR_LNAME_BASE,     STR_LNAME_ALT_BASE, STR_LNAME_RGB_HW,   STR_LNAME_SYM_CHRD,
        STR_LNAME_BRACKETS, STR_LNAME_NUMPAD,   STR_LNAME_NAV_EDIT, STR_LNAME_FN_LOW,
        STR_LNAME_FN_HIGH,  STR_LNAME_MEDIA,    STR_LNAME_MOUSE,    STR_LNAME_MEH_LOW,
        STR_LNAME_MEH_HIGH, STR_LNAME_HYPR_LOW, STR_LNAME_HYPR_HI,  STR_LNAME_SYSTEM
    };

    oled_write_ln(PSTR("Unicorne Master"), false);

    uint8_t current_layer = get_highest_layer(layer_state);
    char display_buffer[17];

    if (current_layer < 16) {
        snprintf(display_buffer, sizeof(display_buffer), "L%d: %s", current_layer, layer_names[current_layer]);
    } else {
        snprintf(display_buffer, sizeof(display_buffer), "Layer: %d", current_layer);
    }
    oled_write_ln(display_buffer, false);

    char live_mod_name[17];
    evaluate_modifier_text(live_mod_name, sizeof(live_mod_name));

    char mod_buffer[17];
    snprintf(mod_buffer, sizeof(mod_buffer), "> %s", live_mod_name);
    oled_write_ln(mod_buffer, false);

    return false;
}
#endif

// ==========================================================================
// 6. 1-to-1 Dedicated Modifier Underglow Engine (Bitmask Switch Routing)
// ==========================================================================
#ifdef RGB_MATRIX_ENABLE

#define ASSIGN_RGB(r_var, g_var, b_var, rgb_macro) do { \
    uint8_t _c[] = {rgb_macro}; \
    r_var = _c[0]; g_var = _c[1]; b_var = _c[2]; \
} while(0)

#define ASSIGN_RGB_ARR(arr, rgb_macro) do { \
    uint8_t _c[] = {rgb_macro}; \
    arr[0] = _c[0]; arr[1] = _c[1]; arr[2] = _c[2]; \
} while(0)

bool rgb_matrix_indicators_user(void) {
    uint8_t mods = get_mods();
#ifdef ONE_SHOT_ENABLE
    mods |= get_oneshot_mods();
#endif

    uint8_t layer_r = 0, layer_g = 0, layer_b = 0;
    uint8_t current_layer = get_highest_layer(layer_state);

    switch (current_layer) {
        case 0:  ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_RUBY_RED);        break;
        case 1:  ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_ELECTRIC_LIME);   break;
        case 2:  ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_TRUE_BLUE);       break;
        case 3:  ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_CYBER_YELLOW);    break;
        case 4:  ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_DEEP_MAGENTA);    break;
        case 5:  ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_HYPER_CYAN);     break;
        case 6:  ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_SAFETY_ORANGE);   break;
        case 7:  ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_TOXIC_GREEN);    break;
        case 8:  ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_NEON_PURPLE);    break;
        case 9:  ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_HOT_PINK);       break;
        case 10: ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_MINT_ICE);       break;
        case 11: ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_SKY_BLUE);       break;
        case 12: ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_PALE_GOLD);      break;
        case 13: ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_WILD_ORCHID);    break;
        case 14: ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_PURE_WHITE);     break;
        case 15: ASSIGN_RGB(layer_r, layer_g, layer_b, RGB_WARM_CORAL);     break;
        default: return true;
    }

    if (mods == 0) {
        for (uint8_t i = 0; i < 8; i++) {
            rgb_matrix_set_color(i, layer_r, layer_g, layer_b);
        }
        return false;
    }

    uint8_t led_colors[8][3];
    for (uint8_t i = 0; i < 8; i++) {
        led_colors[i][0] = layer_r;
        led_colors[i][1] = layer_g;
        led_colors[i][2] = layer_b;
    }

    uint8_t left_hand_mods = mods & (MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT) | MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI));

    switch (left_hand_mods) {
        case MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT) | MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI):
        case MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT) | MOD_BIT(KC_LALT):
            for (uint8_t i = 0; i < 4; i++) {
                led_colors[i][0] = 0; led_colors[i][1] = 160; led_colors[i][2] = 255;
            }
            break;
        default:
            for (uint8_t i = 0; i < 4; i++) {
                switch (mods & (1 << i)) {
                    case MOD_BIT(KC_LCTL): ASSIGN_RGB_ARR(led_colors[0], RGB_BLAZE_ORANGE); break;
                    case MOD_BIT(KC_LSFT): ASSIGN_RGB_ARR(led_colors[1], RGB_HOT_MAGENTA);  break;
                    case MOD_BIT(KC_LALT): ASSIGN_RGB_ARR(led_colors[2], RGB_NEON_LIME);    break;
                    case MOD_BIT(KC_LGUI): ASSIGN_RGB_ARR(led_colors[3], RGB_ELECTRIC_BLUE); break;
                }
            }
            break;
    }

    for (uint8_t i = 4; i < 8; i++) {
        switch (mods & (1 << i)) {
            case MOD_BIT(KC_RGUI): ASSIGN_RGB_ARR(led_colors[4], RGB_DEEP_VIOLET);    break;
            case MOD_BIT(KC_RALT): ASSIGN_RGB_ARR(led_colors[5], RGB_CYBER_YELLOW_M); break;
            case MOD_BIT(KC_RSFT): ASSIGN_RGB_ARR(led_colors[6], RGB_VIVID_CYAN);     break;
            case MOD_BIT(KC_RCTL): ASSIGN_RGB_ARR(led_colors[7], RGB_CRIMSON_RED);    break;
        }
    }

    for (uint8_t i = 0; i < 8; i++) {
        rgb_matrix_set_color(i, led_colors[i][0], led_colors[i][1], led_colors[i][2]);
    }

    return false;
}
#endif

// ==========================================================================
// 7. Native Achordion / Bilateral Combinations Core Logic (Guard Clauses)
// ==========================================================================
bool hold_optional_hands_user(uint16_t keycode, keyrecord_t *record, uint8_t row, uint8_t col) {
    if (keycode < QK_MOD_TAP || keycode > QK_MOD_TAP_MAX) {
        return true;
    }

#if defined(SPLIT_KEYBOARD)
    uint8_t highest_active_hand = (record->event.key.row < (MATRIX_ROWS / 2));
    uint8_t targeted_hand       = (row < (MATRIX_ROWS / 2));

    if (highest_active_hand != targeted_hand) {
        return false;
    }
#endif

    return true;
}
