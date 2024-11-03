#pragma once
#include "Scene/ECS/component.h"



class MeshComponent : public Component {
    int modelIndex;

public:
    MeshComponent();
    void setIndex(int index);
    int getIndex();

};
