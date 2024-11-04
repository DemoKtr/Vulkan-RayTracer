#include "Scene/ECS/components/componentFabric.h"


std::unordered_map<ComponentType, std::function<std::shared_ptr<Component>()>> createComponentFactory() {
        return {
            { ComponentType::Transform, []() { return std::make_shared<TransformComponent>(); } },
            { ComponentType::Mesh,      []() { return std::make_shared<MeshComponent>(); } },
            { ComponentType::Texture,    []() { return std::make_shared<TextureComponent>(); } }//,
            //{ ComponentType::Physics,   []() { return std::make_shared<PhysicsComponent>(); } },
            //{ ComponentType::Script,    []() { return std::make_shared<ScriptComponent>(); } }
        };
}


std::shared_ptr<Component> createComponent(ComponentType type)
{
    static auto componentFactory = createComponentFactory();
    auto it = componentFactory.find(type);
    if (it != componentFactory.end()) {
        return it->second();
    }
    else {
        std::cerr << "Undefine Component" << std::endl;
        return nullptr;
    }
}
