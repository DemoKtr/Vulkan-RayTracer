#pragma once
#include "Scene/ECS/component.h"



class meshComponent : public Component {
    int modelIndex;

public:
    meshComponent();
    void setIndex(int index);
    int getIndex();

};
