#include "View/vkMesh/meshesManager.h"
#include "Scene/ECS/components/components.h"

vkMesh::MeshesManager::MeshesManager(SceneObject* root, ecs::ECS* ecs) {
	// Rekurencyjnie przechodzimy przez drzewo sceny i dodajemy obiekty z komponentem Mesh
	addMeshesRecursively(root,ecs);
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
            int meshIndex = meshComponent->getIndex();
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

void vkMesh::MeshesManager::updateMeshIndex(SceneObject* obj, int newIndex, ecs::ECS* ecs) {
    if (!obj || !ecs) return;  // Sprawdzenie, czy wskaŸniki s¹ wa¿ne

    // Sprawdzamy, czy obiekt ma komponent Mesh
    if (!ecs->hasComponent<MeshComponent>(obj->id)) return;

    // Pobieramy obecny indeks mesha
    auto meshComponent = ecs->getComponent<MeshComponent>(obj->id);
    int currentIndex = meshComponent->getIndex();

    // Sprawdzamy, czy indeks siê zmieni³
    if (currentIndex != newIndex) {
        // Usuwamy stary wpis z `modelMatrices` dla obecnego indeksu
        auto it = modelMatrices.find(currentIndex);
        if (it != modelMatrices.end()) {
            auto& entries = it->second;
            entries.erase(
                std::remove_if(entries.begin(), entries.end(),
                    [obj](const auto& entry) {
                        return entry.sceneObject == obj;
                    }),
                entries.end()
            );

            // Usuniêcie wpisu z mapy, jeœli lista dla danego indeksu jest pusta
            if (entries.empty()) {
                modelMatrices.erase(it);
            }
        }

        // Ustawiamy nowy indeks w komponencie
        meshComponent->setIndex(newIndex);

        // Sprawdzamy, czy obiekt ma komponent Transform i dodajemy nowy wpis do `modelMatrices`
        if (ecs->hasComponent<TransformComponent>(obj->id)) {
            glm::mat4* newModelMatrix = ecs->getComponent<TransformComponent>(obj->id)->getModifyableTransform().getModelMatrixPionter();
            modelMatrices[newIndex].push_back({ obj, newModelMatrix });
        }
    }
}


