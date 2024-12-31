#pragma once
#include "Scene/Objects/Lights/Light.h"

class PointLight : public Light {
private:
    glm::vec3 position;
    float radius; // Range of the light

public:
    PointLight(const std::string& name, const glm::vec3& color, float intensity, const glm::vec3& position, float radius);


    void setPosition(const glm::vec3& newPosition);
    const glm::vec3& getPosition() const;

    void setRadius(float newRadius);
    float getRadius() const;

    void renderInGUI() override;

    void update(float deltaTime) override;
};