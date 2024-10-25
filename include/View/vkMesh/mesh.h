#pragma once
#include "View/vkVertexData/vertex.h"
namespace vkMesh {

	struct VertexBuffers {
		std::vector<vkGeometry::Vertex> vertices;
		std::vector<int> indicies;
	};

	class Mesh {
		
		
	public:
		std::vector<vkGeometry::Vertex> vertices;
		std::vector<uint32_t> indices;
		// constructor
		Mesh(std::vector<vkGeometry::Vertex> vertices, std::vector<uint32_t> indices);

		void printVertices();
	};

}