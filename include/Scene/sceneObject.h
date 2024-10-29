#pragma once
#include "config.h"
#include <glm/fwd.hpp>
#include "Scene/Transform.h"
#include "Scene/ECS/ecs.h"
#include <string>



class SceneObject{

	std::string name;
	
	public:

		ecs::Entity id;
		std::vector<SceneObject*> children;
		SceneObject* parent = nullptr;

		SceneObject(ecs::ECS* ecs, SceneObject* parent);
		SceneObject(ecs::ECS* ecs);
		~SceneObject();
		void draw();
		void addChild(SceneObject* obj);
		void update(float deltaTime);


		void removeObject();

		std::string getName();
};
