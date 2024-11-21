#pragma once
#include "Scene/ECS/component.h"



class MeshComponent : public Component {
    uint64_t modelIndex;

public:
    MeshComponent();
    void setIndex(uint64_t index);
    uint64_t getIndex();

};
