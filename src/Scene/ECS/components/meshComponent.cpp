#include "Scene/ECS/components/meshComponent.h"

MeshComponent::MeshComponent(){
	modelIndex = -1;
	type = ComponentType::Mesh;
}

void MeshComponent::setIndex(int index) {
	modelIndex = index;
}

int MeshComponent::getIndex() {
	return modelIndex;
}