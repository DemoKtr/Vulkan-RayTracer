#include "Scene/Objects/Lights/SpotLight.h"

SpotLight::SpotLight(const std::string& name, const glm::vec3& color, float intensity, const glm::vec3& position, const glm::vec3& direction, float cutOffAngle, float outerCutOffAngle)
	: Light(name, color, intensity), position(position), direction(glm::normalize(direction)), cutOffAngle(cutOffAngle), outerCutOffAngle(outerCutOffAngle) {

}

void SpotLight::setPosition(const glm::vec3& newPosition) {
	position = newPosition;
}

const glm::vec3& SpotLight::getPosition() const {
	return position;
}

void SpotLight::setDirection(const glm::vec3& newDirection) {
	direction = glm::normalize(newDirection);
}

const glm::vec3& SpotLight::getDirection() const{
	return direction;
}

void SpotLight::setCutOffAngle(float newCutOffAngle) {
	cutOffAngle = newCutOffAngle;
}

float SpotLight::getCutOffAngle() const {
	return cutOffAngle;
}

void SpotLight::setOuterCutOffAngle(float newOuterCutOffAngle) {
	outerCutOffAngle = newOuterCutOffAngle;
}

float SpotLight::getOuterCutOffAngle() const {
	return outerCutOffAngle;
}

void SpotLight::renderInGUI() {
	std::cout << "Rendering SpotLight GUI for: " << name << std::endl;
	// Example: ImGui sliders could be added here
}

void SpotLight::update(float deltaTime) {
}

