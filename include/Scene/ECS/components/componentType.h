#pragma once

enum class ComponentType {
    None = -1,
    Transform = 0,
    Mesh = 1,
    Render = 2,
    Physics = 3,
    Script = 4,
    // Dodaj inne typy komponent�w
    Count // Licznik wszystkich typ�w
};