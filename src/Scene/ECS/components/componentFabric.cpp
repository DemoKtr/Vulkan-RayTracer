#include "Scene/ECS/components/componentFabric.h"


// Fabryka komponentów (szablonowe podejœcie)
template <typename T>
std::shared_ptr<T> createComponent() {
    return std::make_shared<T>();
}

template <typename T>
std::shared_ptr<T> createComponentByType(ComponentType type) {
    switch (type) {
    case ComponentType::Transform:
        return createComponent<TransformComponent>();
    case ComponentType::Mesh:
        return createComponent<MeshComponent>();
    case ComponentType::Texture:
        return createComponent<TextureComponent>();
    case ComponentType::Particle:
        return createComponent<ParticleComponent>();
    default:
        std::cerr << "Undefined ComponentType" << std::endl;
        return nullptr;
    }
}
