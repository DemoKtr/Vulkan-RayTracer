#include "View/vkMesh/mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <View/vkVertexData/vertex.h>
#include <Converters/assimpConverter.h>

vkMesh::Mesh::Mesh(std::vector<vkGeometry::Vertex> vertices, std::vector<uint32_t> indices) {
	this->vertices = vertices;
	this->indices = indices;
}
