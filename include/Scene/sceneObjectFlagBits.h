#pragma once
#ifndef SCENEOBJECTFLAG_H
#define SCENEOBJECTFLAG_H
#include "config.h"
namespace scene {

    enum class SceneObjectFlagBits : uint8_t {
        rendering = 1,
        particle = 2,
        Component3 = 3,
        Component4 = 4,
        Component5 = 5,
        Component6 = 6,
        Component7 = 7,
        Component8 = 8
    };

    void updateComponent(SceneObjectFlagBits flag, uint8_t& componentMask);

    void resetComponent(SceneObjectFlagBits flag, uint8_t& componentMask);

    bool isComponentUpdated(SceneObjectFlagBits flag, uint8_t& componentMask);

    void setAllComponentsUpdated(uint8_t& componentMask);
}
#endif

