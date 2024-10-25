#include "Scene/ECS/components/meshComponent.h"
#include "View/vkMesh/meshTypes.h"

meshComponent::meshComponent(){
	modelIndex = -1;
	type = ComponentType::Mesh;
}

void meshComponent::setIndex(int index) {
	modelIndex = index;
}

int meshComponent::getIndex() {
	return modelIndex;
}