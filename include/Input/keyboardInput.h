#pragma once

namespace input{

enum KeyboardInput : uint64_t {
  // Klawisze literowe
    KEY_A = 1ULL << 0,   KEY_B = 1ULL << 1,   KEY_C = 1ULL << 2,   KEY_D = 1ULL << 3,
    KEY_E = 1ULL << 4,   KEY_F = 1ULL << 5,   KEY_G = 1ULL << 6,   KEY_H = 1ULL << 7,
    KEY_I = 1ULL << 8,   KEY_J = 1ULL << 9,   KEY_K = 1ULL << 10,  KEY_L = 1ULL << 11,
    KEY_M = 1ULL << 12,  KEY_N = 1ULL << 13,  KEY_O = 1ULL << 14,  KEY_P = 1ULL << 15,
    KEY_Q = 1ULL << 16,  KEY_R = 1ULL << 17,  KEY_S = 1ULL << 18,  KEY_T = 1ULL << 19,
    KEY_U = 1ULL << 20,  KEY_V = 1ULL << 21,  KEY_W = 1ULL << 22,  KEY_X = 1ULL << 23,
    KEY_Y = 1ULL << 24,  KEY_Z = 1ULL << 25,
    // Cyfry
    KEY_0 = 1ULL << 26,  KEY_1 = 1ULL << 27,  KEY_2 = 1ULL << 28,  KEY_3 = 1ULL << 29,
    KEY_4 = 1ULL << 30,  KEY_5 = 1ULL << 31,  KEY_6 = 1ULL << 32,  KEY_7 = 1ULL << 33,
    KEY_8 = 1ULL << 34,  KEY_9 = 1ULL << 35,
    // Klawisze funkcyjne
    KEY_F1 = 1ULL << 36,  KEY_F2 = 1ULL << 37,  KEY_F3 = 1ULL << 38,  KEY_F4 = 1ULL << 39,
    KEY_F5 = 1ULL << 40,  KEY_F6 = 1ULL << 41,  KEY_F7 = 1ULL << 42,  KEY_F8 = 1ULL << 43,
    KEY_F9 = 1ULL << 44,  KEY_F10 = 1ULL << 45, KEY_F11 = 1ULL << 46, KEY_F12 = 1ULL << 47,
    // Symbole
    KEY_COMMA = 1ULL << 48,    KEY_PERIOD = 1ULL << 49,   KEY_SLASH = 1ULL << 50,
    KEY_SEMICOLON = 1ULL << 51, KEY_QUOTE = 1ULL << 52,   KEY_BRACKET_OPEN = 1ULL << 53,
    KEY_BRACKET_CLOSE = 1ULL << 54, KEY_BACKSLASH = 1ULL << 55,
    // Specjalne
    KEY_SPACE = 1ULL << 56,   KEY_TAB = 1ULL << 57,    KEY_ENTER = 1ULL << 58,
    KEY_BACKSPACE = 1ULL << 59, KEY_ESC = 1ULL << 60,  KEY_SHIFT = 1ULL << 61,
    KEY_CTRL = 1ULL << 62,    KEY_ALT = 1ULL << 63
};

}