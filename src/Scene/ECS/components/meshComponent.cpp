#include "Scene/ECS/components/meshComponent.h"

MeshComponent::MeshComponent(){
	modelIndex = -1;
	type = ComponentType::Mesh;
}

void MeshComponent::setIndex(uint64_t index) {
	modelIndex = index;
}

uint64_t MeshComponent::getIndex() {
	return modelIndex;
}

nlohmann::json MeshComponent::serialize() const
{
	return nlohmann::json();
}
