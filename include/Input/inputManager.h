#pragma once
#ifndef INPUT_H
#define INPUT_H
#include "config.h"
#include "keyboardInput.h"
#include "mouseInput.h"
#include "Input/keyMapping.h"
namespace input {

    class InputManager {
    private:
        uint64_t keyboardState = 0;
        uint8_t mouseState = 0;       // Stan wejœcia (klawisze/myszy)
        double mouseX, mouseY;     // Pozycja myszy
        bool mouseUpdated;         // Czy pozycja myszy zosta³a zaktualizowana?

    public:
        InputManager() :  mouseX(0.0), mouseY(0.0), mouseUpdated(false) {}

        // Aktualizacja stanu klawiatury
        void updateKeyboardState(KeyboardInput key, bool pressed);

        // Aktualizacja stanu myszy
        void updateMouseState(MouseInput button, bool pressed);

        void updateMousePosition(double x, double y);

        // Sprawdzanie stanu klawisza lub przycisku myszy
        bool isKeyPressed(KeyboardInput key);

        bool isMouseButtonPressed(MouseInput button);

        std::pair<double, double> getMousePosition() const;

        bool wasMouseUpdated() const;

        void resetMouseUpdate();
    };
}



#endif // !APP_H