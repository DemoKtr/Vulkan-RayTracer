#include "View/vkMesh/mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <View/vkVertexData/vertex.h>
#include <Converters/assimpConverter.h>
#include <iostream>

vkMesh::Mesh::Mesh(std::vector<vkGeometry::Vertex> vertices, std::vector<uint32_t> indices) {
	this->vertices = vertices;
	this->indices = indices;
}

void vkMesh::Mesh::printVertices() {
	size_t i = 0;
	for (vkGeometry::Vertex v : vertices) {
		std::cout << "Vertex number:" << i << std::endl;
		std::cout << "x: "<<v.Position.x << " y: " << v.Position.y << " z: " << v.Position.z << std::endl;
	}
}
