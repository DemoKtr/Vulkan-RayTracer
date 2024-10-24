#include "View/vkMesh/meshLoader.h"
#include <Converters/assimpConverter.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>

vkMesh::MeshLoader::MeshLoader(const char* path) {
	loadModel(path);
}

void vkMesh::MeshLoader::loadModel(const char* path) {
	// read file via ASSIMP
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	// retrieve the directory path of the filepath


	//directory = path.substr(0, pathStr.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

void vkMesh::MeshLoader::processNode(aiNode* node, const aiScene* scene) {
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

vkMesh::Mesh vkMesh::MeshLoader::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<vkGeometry::Vertex> vertices;
	std::vector<unsigned int> indices;


	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vkGeometry::Vertex vertex;
		vertex.Position = glm::vec4(Converter::AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]), 1.0f);
		vertex.Normal = glm::vec4(Converter::AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]), 1.0f);

		if (mesh->mTextureCoords[0])
		{
			glm::vec4 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vec.z = 0.0f;
			vec.w = 1.0f;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}



	return vkMesh::Mesh(vertices, indices);
}
