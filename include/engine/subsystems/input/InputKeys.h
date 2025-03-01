/**************************************************************************************************
 * @file   InputKeys.h
 * @author Valentin Dumitru
 * @date   2024-04-17
 * @brief  @TODO Add description of this file if needed
 *
 * Copyright (c) 2024 Valentin Dumitru. Licensed under the MIT License.
 * See LICENSE.txt in the project root for license information.
 **************************************************************************************************/

#pragma once

#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_mouse.h"

namespace GLESC::Input {
    /**
     * @brief Enum for the different actions a key can have.
     * @details This is used to determine the action that should be taken when a key is pressed.
     */
    enum class KeyAction {
        ONCE_RELEASED,
        ONCE_PRESSED,
        ONGOING_PRESSED,
        ONGOING_RELEASED,
    };

    /**
     * @brief Mapping of keys to SDL keycodes.
     * @details This ensures the input subsystem of the engine is independent of the SDL library.
     */
    enum class Key {
        LEFT_CLICK = SDL_BUTTON_LEFT,
        RIGHT_CLICK = SDL_BUTTON_RIGHT,
        MIDDLE_CLICK = SDL_BUTTON_MIDDLE,
        ESCAPE = SDLK_ESCAPE,
        SPACE = SDLK_SPACE,
        LEFT = SDLK_LEFT,
        RIGHT = SDLK_RIGHT,
        UP = SDLK_UP,
        DOWN = SDLK_DOWN,
        W = SDLK_w,
        A = SDLK_a,
        S = SDLK_s,
        D = SDLK_d,
        Q = SDLK_q,
        E = SDLK_e,
        R = SDLK_r,
        F = SDLK_f,
        T = SDLK_t,
        G = SDLK_g,
        Y = SDLK_y,
        H = SDLK_h,
        U = SDLK_u,
        J = SDLK_j,
        I = SDLK_i,
        K = SDLK_k,
        O = SDLK_o,
        L = SDLK_l,
        P = SDLK_p,
        Z = SDLK_z,
        X = SDLK_x,
        C = SDLK_c,
        V = SDLK_v,
        B = SDLK_b,
        N = SDLK_n,
        M = SDLK_m,
        ONE = SDLK_1,
        TWO = SDLK_2,
        THREE = SDLK_3,
        FOUR = SDLK_4,
        FIVE = SDLK_5,
        SIX = SDLK_6,
        SEVEN = SDLK_7,
        EIGHT = SDLK_8,
        NINE = SDLK_9,
        ZERO = SDLK_0,
        F1 = SDLK_F1,
        F2 = SDLK_F2,
        F3 = SDLK_F3,
        F4 = SDLK_F4,
        F5 = SDLK_F5,
        F6 = SDLK_F6,
        F7 = SDLK_F7,
        F8 = SDLK_F8,
        F9 = SDLK_F9,
        F10 = SDLK_F10,
        F11 = SDLK_F11,
        F12 = SDLK_F12,
        F13 = SDLK_F13,
        F14 = SDLK_F14,
        F15 = SDLK_F15,
        F16 = SDLK_F16,
        F17 = SDLK_F17,
        F18 = SDLK_F18,
        F19 = SDLK_F19,
        F20 = SDLK_F20,
        F21 = SDLK_F21,
        F22 = SDLK_F22,
        F23 = SDLK_F23,
        F24 = SDLK_F24,
        NUMLOCK = SDLK_NUMLOCKCLEAR,
        CAPSLOCK = SDLK_CAPSLOCK,
        SCROLLLOCK = SDLK_SCROLLLOCK,
        RIGHT_SHIFT = SDLK_RSHIFT,
        LEFT_SHIFT = SDLK_LSHIFT,
        RIGHT_CTRL = SDLK_RCTRL,
        LEFT_CTRL = SDLK_LCTRL,
        RIGHT_ALT = SDLK_RALT,
        LEFT_ALT = SDLK_LALT,
        RIGHT_GUI = SDLK_RGUI,
        LEFT_GUI = SDLK_LGUI,
        LEFT_BRACKET = SDLK_LEFTBRACKET,
        RIGHT_BRACKET = SDLK_RIGHTBRACKET,
        SEMICOLON = SDLK_SEMICOLON,
        COMMA = SDLK_COMMA,
        PERIOD = SDLK_PERIOD,
        QUOTE = SDLK_QUOTE,
        SLASH = SDLK_SLASH,
        BACKSLASH = SDLK_BACKSLASH,
        BACKQUOTE = SDLK_BACKQUOTE,
        MINUS = SDLK_MINUS,
        EQUALS = SDLK_EQUALS,
        RETURN = SDLK_RETURN,
        TAB = SDLK_TAB,
        BACKSPACE = SDLK_BACKSPACE,
        INSERT = SDLK_INSERT,
        PAGE_UP = SDLK_PAGEUP,
        PAGE_DOWN = SDLK_PAGEDOWN,
        HOME = SDLK_HOME,
        END = SDLK_END,
        CAPS_LOCK = SDLK_CAPSLOCK,
        SCROLL_LOCK = SDLK_SCROLLLOCK,
        PRINT_SCREEN = SDLK_PRINTSCREEN,
        PAUSE = SDLK_PAUSE,
        NUM_LOCK = SDLK_NUMLOCKCLEAR,
        CLEAR = SDLK_CLEAR,
        APPLICATION = SDLK_APPLICATION,
        POWER = SDLK_POWER,
        MOUSE_BUTTON_LEFT = SDL_BUTTON_LEFT,
        MOUSE_BUTTON_MIDDLE = SDL_BUTTON_MIDDLE,
        MOUSE_BUTTON_RIGHT = SDL_BUTTON_RIGHT,
    };
}