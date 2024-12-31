#pragma once

#include "config.h"

// Base Light class
class Light {
protected:
    std::string name; // Optional name for the light
    glm::vec3 color;  // Light color
    float intensity;  // Intensity of the light

public:
    Light(const std::string& name, const glm::vec3& color, float intensity)
        : name(name), color(color), intensity(intensity) {}

    virtual ~Light() = default;

    // Virtual methods to override
    virtual void renderInGUI() = 0; // Rendering settings in GUI
    virtual void update(float deltaTime) = 0; // Per-frame update

    // Common getters and setters
    void setColor(const glm::vec3& newColor);
    void setIntensity(float newIntensity); 
    const glm::vec3& getColor() const; 
    float getIntensity() const; 
};