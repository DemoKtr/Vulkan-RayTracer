#pragma once

namespace input{

enum MouseInput : uint8_t {
    MOUSE_LEFT = 1 << 0,    // Lewy przycisk myszy
    MOUSE_RIGHT = 1 << 1,   // Prawy przycisk myszy
    MOUSE_MIDDLE = 1 << 2,  // Środkowy przycisk (scroll)
    MOUSE_EXTRA1 = 1 << 3,  // Przyciski dodatkowe myszy
    MOUSE_EXTRA2 = 1 << 4
};
}