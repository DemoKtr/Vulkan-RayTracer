#include "Scene/ECS/components/transformComponent.h"

TransformComponent::TransformComponent(){
	type = ComponentType::Transform;
}

nlohmann::json TransformComponent::serialize() const {
	return nlohmann::json();
}

TransformComponent::~TransformComponent()
{
}
