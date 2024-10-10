#pragma once
#include <cstddef>

constexpr std::size_t MAX_COMPONENTS = 32;
struct Component {
    virtual ~Component() = default;
};