#pragma once
#include "config.h"
#include "View/vkMesh/vertexMenagerie.h"

namespace vkAcceleration {
	struct AccelerationStructuresBuilderInfo {
		vk::Device device;
		vkMesh::VertexMenagerie* verticesMenagerie;
	};

	void build_structures(AccelerationStructuresBuilderInfo info);

}