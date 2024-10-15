#pragma once
#include <cstddef>
#include "components/componentType.h"

constexpr std::size_t MAX_COMPONENTS = 32;
struct Component {
    virtual ~Component() = default;
public:
    ComponentType type;
    // Metoda get dla type
    ComponentType getType() const {
        return type;
    }
};