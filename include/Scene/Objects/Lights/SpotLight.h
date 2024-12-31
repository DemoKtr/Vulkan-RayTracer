#pragma once
#include "Scene/Objects/Lights/Light.h"

class SpotLight : public Light {
private:
    glm::vec3 position;
    glm::vec3 direction;
    float cutOffAngle;   // Inner cone angle
    float outerCutOffAngle; // Outer cone angle

public:
    SpotLight(const std::string& name, const glm::vec3& color, float intensity, const glm::vec3& position, const glm::vec3& direction, float cutOffAngle, float outerCutOffAngle);
    

    void setPosition(const glm::vec3& newPosition);
    const glm::vec3& getPosition() const;

    void setDirection(const glm::vec3& newDirection);
    const glm::vec3& getDirection() const;

    void setCutOffAngle(float newCutOffAngle);
    float getCutOffAngle() const;

    void setOuterCutOffAngle(float newOuterCutOffAngle);
    float getOuterCutOffAngle() const;

    void renderInGUI() override;

    void update(float deltaTime) override;
};
