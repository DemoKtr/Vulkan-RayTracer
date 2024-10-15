#pragma once
#include "config.h"
#include <glm/fwd.hpp>
#include "Scene/Transform.h"
#include "Scene/ECS/ecs.h"
#include <string>
struct objectMaterial {
	meshTypes meshType;
	char* model{};
	char* material{};
	char* diffuse{};
	char* normalMap{};
	char* armMap{};
	char* depthMap{};

	void setFilenames(const char* newModel, const char* newMaterial, const char* newDiffuse, const char* newNormalMap, const char* newArmMap, const char* newDepthMap) {
		// Zwolnienie poprzednio alokowanej pami�ci (je�li istnieje)


		// Alokowanie nowej pami�ci i kopiowanie �a�cuch�w znak�w
		model = _strdup(newModel);
		material = _strdup(newMaterial);
		diffuse = _strdup(newDiffuse);
		normalMap = _strdup(newNormalMap);
		armMap = _strdup(newArmMap);
		depthMap = _strdup(newDepthMap);

	}



	void clear() {
		delete[] model;
		delete[] material;
		delete[] diffuse;
		delete[] normalMap;
		delete[] armMap;
		delete[] depthMap;
	}

};



class SceneObject{

	std::string name;
	
	public:

		ecs::Entity id;
		std::vector<SceneObject*> children;
		SceneObject* parent = nullptr;

		SceneObject(ecs::ECS* ecs, SceneObject* parent);
		SceneObject(ecs::ECS* ecs);
		void draw();
		void addChild(SceneObject* obj);
		void update(float deltaTime);
		objectMaterial objMaterial;


		std::string getName();
};
