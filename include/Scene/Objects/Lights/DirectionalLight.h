#pragma once
#include "Scene/Objects/Lights/Light.h"

class DirectionalLight : public Light {
private:
    glm::vec3 direction;

public:
    DirectionalLight(const std::string& name, const glm::vec3& color, float intensity, const glm::vec3& direction);
         

    void setDirection(const glm::vec3& newDirection);
    const glm::vec3& getDirection() const;

    void renderInGUI() override;

    void update(float deltaTime) override;
};
