#include "Scene/Objects/Lights/PointLight.h"




PointLight::PointLight(const std::string& name, const glm::vec3& color, float intensity, const glm::vec3& position, float radius)
	: Light(name, color, intensity), position(position), radius(radius) {

}

void PointLight::setPosition(const glm::vec3& newPosition) {
	position = newPosition;
}

const glm::vec3& PointLight::getPosition() const {
	return position;
}

void PointLight::setRadius(float newRadius) {
	radius = newRadius;
}

float PointLight::getRadius() const {
	return radius;
}

void PointLight::renderInGUI() {
	std::cout << "Rendering PointLight GUI for: " << name << std::endl;
	// Example: ImGui sliders could be added here
}

void PointLight::update(float deltaTime) {

}
