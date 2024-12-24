#include "Scene/ECS/components/meshComponent.h"

MeshComponent::MeshComponent(){

	type = ComponentType::Mesh;
	setIndex(17052657060290881606);
}

void MeshComponent::setIndex(uint64_t index) {
	modelIndex = index;
}

uint64_t MeshComponent::getIndex() {
	return modelIndex;
}

nlohmann::json MeshComponent::serialize() const {
    nlohmann::json json;

    // Serializacja pola modelIndex
    json["modelIndex"] = modelIndex;

    return json;
}
