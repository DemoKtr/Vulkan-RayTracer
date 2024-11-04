#pragma once

enum class ComponentType {
    None = -1,
    Transform = 0,
    Mesh = 1,
    Texture = 2,
    Render = 3,
    Physics = 4,
    Script = 5,
    // Dodaj inne typy komponentów
    Count // Licznik wszystkich typów
};