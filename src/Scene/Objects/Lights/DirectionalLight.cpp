#include "Scene/Objects/Lights/DirectionalLight.h"

DirectionalLight::DirectionalLight(const std::string& name, const glm::vec3& color, float intensity, const glm::vec3& direction)
	: Light(name, color, intensity), direction(glm::normalize(direction))
{
}

void DirectionalLight::setDirection(const glm::vec3& newDirection){
	direction = glm::normalize(newDirection);
}

const glm::vec3& DirectionalLight::getDirection() const {
	return direction;
}

void DirectionalLight::renderInGUI() {
	std::cout << "Rendering DirectionalLight GUI for: " << name << std::endl;
	// Example: ImGui sliders could be added here
}

void DirectionalLight::update(float deltaTime) {
}
