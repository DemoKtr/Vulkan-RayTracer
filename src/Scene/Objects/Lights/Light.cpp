#include "Scene/Objects/Lights/Light.h"

void Light::setColor(const glm::vec3& newColor) {
	color = newColor;
}

void Light::setIntensity(float newIntensity) {
	intensity = newIntensity;
}

const glm::vec3& Light::getColor() const {
	return color;
}

float Light::getIntensity() const {
	return intensity;
}
