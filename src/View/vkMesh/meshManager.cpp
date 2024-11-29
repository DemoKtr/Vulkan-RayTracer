#include "View/vkMesh/meshesManager.h"
#include "Scene/ECS/components/components.h"
#include <iostream>

vkMesh::MeshesManager::MeshesManager(SceneObject* root, ecs::ECS* ecs) {
	// Rekurencyjnie przechodzimy przez drzewo sceny i dodajemy obiekty z komponentem Mesh
	//addMeshesRecursively(root,ecs);
    addMeshesRecursively(root, ecs);
}

void vkMesh::MeshesManager::addMeshesRecursively(SceneObject* obj, ecs::ECS* ecs) {
    if (!obj) return;

    // Sprawdzamy, czy obiekt ma komponent Mesh
    if (ecs->hasComponent<MeshComponent>(obj->id)) {
        // Pobieramy macierz modelu z komponentu transformacji i dodajemy do mapy
        TransformComponent * transformComponent = ecs->getComponent<TransformComponent>(obj->id).get();

        if (transformComponent != nullptr) {
            // Pobieramy wskaŸnik na MeshComponent i jego indeks

            MeshComponent* meshComponent = ecs->getComponent<MeshComponent>(obj->id).get();
            uint64_t meshIndex = meshComponent->getIndex();
            // Tworzymy MeshData i zapisujemy do mapy
            modelMatrices[meshIndex].push_back( {obj, transformComponent->getModifyableTransform().getModelMatrixPionter() });
        }
    }

    // Rekurencyjnie przeszukujemy dzieci
    for (SceneObject* child : obj->children) {
        addMeshesRecursively(child,ecs);
    }
}

void vkMesh::MeshesManager::removeSceneObject(SceneObject* obj, ecs::ECS* ecs) {
    if (!obj) return;

    for (auto it = modelMatrices.begin(); it != modelMatrices.end(); ) {
        // Filtruj wektor, aby usun¹æ wszystkie `MeshManagerData`, które odnosz¹ siê do `obj`
        auto& dataVector = it->second;
        dataVector.erase(
            std::remove_if(dataVector.begin(), dataVector.end(),
                [obj](const MeshManagerData& data) {
                    return data.sceneObject == obj;
                }),
            dataVector.end()
        );

        // Jeœli wektor jest pusty po usuniêciu, usuñ tak¿e wpis z mapy
        if (dataVector.empty()) {
            it = modelMatrices.erase(it);
        }
        else {
            ++it;
        }
    }
}



void vkMesh::MeshesManager::updateMeshIndex(SceneObject* obj, uint64_t newIndex, ecs::ECS* ecs) {
    if (!obj || !ecs) {
        std::cerr << "[ERROR] Nullptr passed to updateMeshIndex!" << std::endl;
        return;
    }

    if (!ecs->hasComponent<MeshComponent>(obj->id)) {
        std::cerr << "[DEBUG] Object ID " << obj->id << " has no MeshComponent." << std::endl;
        return;
    }

    auto meshComponent = ecs->getComponent<MeshComponent>(obj->id);
    uint64_t currentIndex = meshComponent->getIndex();

    if (currentIndex != newIndex) {
        auto it = modelMatrices.find(currentIndex);
        if (it != modelMatrices.end()) {
            auto& entries = it->second;

            std::cout << "[DEBUG] Current index " << currentIndex
                << " has " << entries.size() << " entries before removal." << std::endl;

            size_t beforeSize = entries.size();
            entries.erase(
                std::remove_if(entries.begin(), entries.end(),
                    [obj](const auto& entry) {
                        return entry.sceneObject && entry.sceneObject->id == obj->id;
                    }),
                entries.end()
            );

            if (entries.empty()) {
                modelMatrices.erase(it);
            }

            std::cout << "[DEBUG] Removed object from index " << currentIndex
                << ". Before: " << beforeSize << ", After: " << entries.size() << std::endl;
        }
        else {
            std::cerr << "[ERROR] Index " << currentIndex << " not found in modelMatrices!" << std::endl;
        }

        meshComponent->setIndex(newIndex);

        if (ecs->hasComponent<TransformComponent>(obj->id)) {
            glm::mat4* newModelMatrix = ecs->getComponent<TransformComponent>(obj->id)->getModifyableTransform().getModelMatrixPionter();
            modelMatrices[newIndex].push_back({ obj, newModelMatrix });
            std::cout << "[DEBUG] Added object to new index " << newIndex << std::endl;
        }
    }
}




void vkMesh::MeshesManager::addMesh(SceneObject* obj, ecs::ECS* ecs) {
    if (!obj) return;

    // Sprawdzamy, czy obiekt ma komponent Mesh
    if (ecs->hasComponent<MeshComponent>(obj->id)) {
        // Pobieramy macierz modelu z komponentu transformacji i dodajemy do mapy
        TransformComponent* transformComponent = ecs->getComponent<TransformComponent>(obj->id).get();

        if (transformComponent != nullptr) {
            // Pobieramy wskaŸnik na MeshComponent i jego indeks
            MeshComponent* meshComponent = ecs->getComponent<MeshComponent>(obj->id).get();
            uint64_t meshIndex = meshComponent->getIndex();
            // Tworzymy MeshData i zapisujemy do mapy
            modelMatrices[meshIndex].push_back({ obj, transformComponent->getModifyableTransform().getModelMatrixPionter() });

        }
    }
}