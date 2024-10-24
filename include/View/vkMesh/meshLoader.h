#pragma once

#include "View/vkMesh/mesh.h"
#include <assimp/scene.h>

namespace vkMesh {
	class MeshLoader {
	public:
		std::vector<Mesh> meshes;
		// constructor, expects a filepath to a 3D model.
		MeshLoader(const char* path);

	private:
		int m_BoneCounter = 0;

		// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
		void loadModel(const char* path);

		// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
		void processNode(aiNode* node, const aiScene* scene);

		vkMesh::Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	};
}