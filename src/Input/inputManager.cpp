#include "Input/inputManager.h"

void input::InputManager::updateKeyboardState(KeyboardInput key, bool pressed) {
    if (pressed) {
        keyboardState |= key; // Ustaw bit
    }
    else {
        keyboardState &= ~key; // Wyzeruj bit
    }
}

void input::InputManager::updateMouseState(MouseInput button, bool pressed) {
    if (pressed) {
        mouseState |= button; // Ustaw bit
    }
    else {
        mouseState &= ~button; // Wyzeruj bit
    }
}

void input::InputManager::updateMousePosition(double x, double y) {
    mouseX = x;
    mouseY = y;
    mouseUpdated = true;
}

bool input::InputManager::isKeyPressed(KeyboardInput key) {
    return (keyboardState & key) != 0;
}

bool input::InputManager::isMouseButtonPressed(MouseInput button) {
    return (mouseState & button) != 0;
}

std::pair<double, double> input::InputManager::getMousePosition() const {
    return { mouseX, mouseY };
}

bool input::InputManager::wasMouseUpdated() const {
    return mouseUpdated;
}

void input::InputManager::resetMouseUpdate() {
    mouseUpdated = false;
}
