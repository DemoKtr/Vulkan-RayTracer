#pragma once
#include "Scene/ECS/ecs.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <iostream>

namespace scripts {

    class Script {

        ecs::ECS* ecs = nullptr;
        ecs::Entity entity;

        std::string libraryPath;
        void* handle;

        void* loadLibrary();
        void closeLibrary();
        void* getFunction(std::string functionName);


    public:

        Script(std::string libraryPath);
        Script(const Script& script);
        Script();
        ~Script();
        

        template <typename T>
        std::shared_ptr<T> getComponent() {
            if (!ecs) {
                throw std::runtime_error("ECS context is not set");
            }
            return ecs->getComponent<T>(entity);
        }
        void setECSContext(ecs::ECS* ecs, ecs::Entity entity);
        void execute_onStart_function();
        void execute_onUpdate_function(float deltaTime);
        void execute_onEnd_function();

    };

}