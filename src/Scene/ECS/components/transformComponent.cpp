#include "Scene/ECS/components/transformComponent.h"

TransformComponent::TransformComponent(){
	type = ComponentType::Transform;
}

nlohmann::json TransformComponent::serialize() const {
    nlohmann::json json;

    // Serializacja pozycji
    json["position"] = { transform.getLocalPosition().x, transform.getLocalPosition().y, transform.getLocalPosition().z };

    // Serializacja rotacji (kwaternion)
    json["rotation"] = { transform.getLocalRotation().w, transform.getLocalRotation().x, transform.getLocalRotation().y, transform.getLocalRotation().z };

    // Serializacja skali
    json["scale"] = { transform.getLocalScale().x, transform.getLocalScale().y, transform.getLocalScale().z};

    return json;
}

TransformComponent::~TransformComponent()
{
}
