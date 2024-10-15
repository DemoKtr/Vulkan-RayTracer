#pragma once
#include "config.h"
#include "Scene/ECS/component.h"
#include "Scene/Transform.h"


class TransformComponent : public Component {
	Transform transform;
	
public:
	TransformComponent();
	
    // Metoda get dla transform
    const Transform& getTransform() const {
        return transform;
    }
    Transform& getModifyableTransform() {
        return transform;
    }

    // Metody set dla transform
    void setPosition(const glm::vec3& position) {
        transform.setLocalPosition(position);
    }

    void setRotation(const glm::vec3 axis,float angle) {
        transform.rotate(axis,angle);  // Zak³adam, ¿e m_rotation jest publiczne
    }

    void setScale(const glm::vec3& scale) {
        transform.setLocalScale(scale);
    }

   



};