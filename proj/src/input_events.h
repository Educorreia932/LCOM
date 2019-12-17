#pragma once

#include <lcom/lcf.h>

/** 
 * @brief 8 bit number where the MSB represents make or break code.
 * @details So we only have 0x7F possibilites
 */
#define LARGEST_SCANCODE 0x7F 
#define EXTRA_MAKECODES 6

/* INPUT EVENT STUFF */

/**
 * @brief This has all possible keys
 */
typedef enum KeyboardMap {
    // 1 Byte scancodes
    KBD_ESC = 0x01,
    KBD_Q = 0x10,
    KBD_W = 0x11,
    KBD_E = 0x12,
    KBD_R = 0x13,
    KBD_T = 0x14,
    KBD_Y = 0x15,
    KBD_U = 0x16,
    KBD_I = 0x17,
    KBD_O = 0x18,
    KBD_P = 0x19,
    KBD_A = 0x1E,
    KBD_S = 0x1F,
    KBD_D = 0x20,
    KBD_F = 0x21,
    KBD_G = 0x22,
    KBD_H = 0x23,
    KBD_J = 0x24,
    KBD_K = 0x25,
    KBD_L = 0x26,
    KBD_Ç = 0x27,
    KBD_Z = 0x2C,
    KBD_X = 0x2D,
    KBD_C = 0x2E,
    KBD_V = 0x2F,
    KBD_B = 0x30,
    KBD_N = 0x31,
    KBD_M = 0x32,
    KBD_KEY_1 = 0x02,
    KBD_KEY_2 = 0x03,
    KBD_KEY_3 = 0x04,
    KBD_KEY_4 = 0x05,
    KBD_KEY_5 = 0x06,
    KBD_KEY_6 = 0x07,
    KBD_KEY_7 = 0x08,
    KBD_KEY_8 = 0x09,
    KBD_KEY_9 = 0x0A,
    KBD_KEY_0 = 0x0B,
    KBD_F1 = 0x3B,
    KBD_F2 = 0x3C,
    KBD_F3 = 0x3D,
    KBD_F4 = 0x3E,
    KBD_F5 = 0x3F,
    KBD_F6 = 0x40,
    KBD_F7 = 0x41,
    KBD_F8 = 0x42,
    KBD_F9 = 0x43,
    KBD_F10 = 0x44,
    KBD_F11 = 0x57,
    KBD_F12 = 0x58,
    KBD_L_SHIFT = 0x2A,
    KBD_R_SHIFT = 0x36,
    KBD_L_CTRL = 0x1D,
    KBD_ALT = 0x38,
    KBD_SPACEBAR = 0x39,
    KBD_NUM_PAD_1 = 0x4F,
    KBD_NUM_PAD_2 = 0x50,
    KBD_NUM_PAD_3 = 0x51,
    KBD_NUM_PAD_4 = 0x4B,
    KBD_NUM_PAD_5 = 0x4C,
    KBD_NUM_PAD_6 = 0x4D,
    KBD_NUM_PAD_7 = 0x47,
    KBD_NUM_PAD_8 = 0x48,
    KBD_NUM_PAD_9 = 0x49,
    KBD_NUM_PAD_0 = 0x52,
    KBD_HIFEN = 0x35,
    KBD_DOT = 0x34,
    KBD_COMMA = 0x33,
    KBD_LESSER_THAN = 0x56,
    KBD_CAPS_LOCK = 0x3A,
    KBD_TAB = 0x0F,
    KBD_LEFT_SLASH = 0x29,

    // 2 byte scancodes, aka special cases
    KBD_ARROW_LEFT = LARGEST_SCANCODE + 1,
    KBD_ARROW_RIGHT = LARGEST_SCANCODE + 2,
    KBD_ARROW_UP = LARGEST_SCANCODE + 3,
    KBD_ARROW_DOWN = LARGEST_SCANCODE + 4,
    KBD_DELETE = LARGEST_SCANCODE + 5,
    KBD_SUPER = LARGEST_SCANCODE + 6

} KeyboardMap_t;

bool get_key(KeyboardMap_t map);
bool get_key_down(KeyboardMap_t map);


void initialize_kbd_input_events();
void free_kbd_input_events();

void kbd_input_events_process_scancode();


void initialize_mouse_input_events();
void free_mouse_input_events();

void mouse_input_events_process_packet();

bool mouse_get_lb_down();
bool mouse_get_lb();
bool mouse_get_rb_down();
bool mouse_get_rb();
int32_t mouse_get_x_delta();
int32_t mouse_get_y_delta();


/**
 * @brief Resets inputs
 */
void reset_kbd_input_state();
void reset_mouse_input_state();
