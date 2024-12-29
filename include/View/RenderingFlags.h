#pragma once
#include <vector>
#include <map>
#include <Scene/ECS/components/components.h>
#include <Scene/sceneObject.h>
constexpr uint8_t FLAG_RENDER_DEFERED_RENDERING = 1 << 0; // bit 0
constexpr uint8_t FLAG_RENDER_SKYBOX = 1 << 1; // bit 1
constexpr uint8_t FLAG_RENDER_LIGHTS = 1 << 2;   // bit 2
constexpr uint8_t FLAG_RENDER_TEST_ONE = 1 << 3;    // bit 3
constexpr uint8_t FLAG_RENDER_TEST_TWO = 1 << 4;    // bit 4
constexpr uint8_t FLAG_RENDER_TEST_THREE = 1 << 5;    // bit 5
constexpr uint8_t FLAG_RENDER_TEST_FOUR = 1 << 6;    // bit 6
constexpr uint8_t FLAG_RENDER_TEST_FIVE = 1 << 7;    // bit 7


enum GroupType : uint8_t {
    UNLIT = 0,
    PBR = 1,
    INACTIVE = 2
};

struct RenderObjects {
    std::map<uint64_t, std::vector<SceneObject*>> unlit;
    std::map<uint64_t, std::vector<SceneObject*>> pbr;
    std::vector<SceneObject*> inactive;

    std::unordered_map<SceneObject*, GroupType> objectToGroup;

    void addObjectToGroup(uint64_t modelId, SceneObject* obj, GroupType group) {
        if (objectToGroup.find(obj) != objectToGroup.end()) {
            GroupType previousGroup = objectToGroup[obj];
            removeObjectFromGroup(modelId, obj, previousGroup);
        }

        switch (group) {
        case UNLIT:
            unlit[modelId].push_back(obj);
            break;
        case PBR:
            pbr[modelId].push_back(obj);
            break;
        case INACTIVE:
            if (std::find(inactive.begin(), inactive.end(), obj) == inactive.end()) {
                inactive.push_back(obj);
            }
            break;
        }
        std::cout << "Mam grupe: " << static_cast<int>(group) << std::endl;
        objectToGroup[obj] = group;
        std::cout << "Jest Inactive: " << inactive.size() << std::endl;
        std::cout << "Jest PBR: " << pbr.size() << std::endl;
        std::cout << "Jest Unlit: " << unlit.size() << std::endl;
    }

    void removeObjectFromGroup(uint64_t modelId, SceneObject* obj, GroupType group) {
        switch (group) {
        case UNLIT:
            if (unlit.count(modelId)) {
                removeFromGroup(unlit[modelId], obj);
                if (unlit[modelId].empty()) {
                    unlit.erase(modelId);
                }
            }
            break;
        case PBR:
            if (pbr.count(modelId)) {
                removeFromGroup(pbr[modelId], obj);
                if (pbr[modelId].empty()) {
                    pbr.erase(modelId);
                }
            }
            break;
        case INACTIVE:
            removeFromGroup(inactive, obj);
            break;
        }
        objectToGroup.erase(obj);
    }

    void removeFromGroup(std::vector<SceneObject*>& group, SceneObject* obj) {
        auto it = std::find(group.begin(), group.end(), obj);
        if (it != group.end()) {
            group.erase(it);
        }
    }

    GroupType getObjectGroup(SceneObject* obj) const {
        // Sprawdzamy, w której grupie jest obiekt
        auto it = objectToGroup.find(obj);
        if (it != objectToGroup.end()) {
            return it->second;
        }
        throw std::runtime_error("Object not found in any group.");
    }

    void changeModelId(ecs::ECS* ecs, uint64_t newModelId, SceneObject* obj) {
        // SprawdŸ, czy obiekt ma MeshComponent
        uint64_t oldModelId;
        MeshComponent* comp = nullptr;
        if (ecs->hasComponent<MeshComponent>(obj->id)) {
            comp = ecs->getComponent<MeshComponent>(obj->id).get();
            oldModelId = comp->getIndex();
        }
        else {
            throw std::runtime_error("Object does not have a MeshComponent.");
        }

        // SprawdŸ, czy obiekt istnieje w grupach
        auto it = objectToGroup.find(obj);
        if (it == objectToGroup.end()) {
            throw std::runtime_error("Object not found in any group.");
        }

        GroupType group = it->second;

        // Jeœli obiekt jest w INACTIVE, modelId nie ma znaczenia
        if (group == INACTIVE) {
            std::cout << "Object is in INACTIVE group. Model ID change is not applicable.\n";
            return;
        }

        // Usuñ obiekt ze starego modelId w bie¿¹cej grupie
        removeObjectFromGroup(oldModelId, obj, group);

        // Dodaj obiekt do nowego modelId w tej samej grupie
        switch (group) {
        case UNLIT:
            unlit[newModelId].push_back(obj);
            break;
        case PBR:
            pbr[newModelId].push_back(obj);
            break;
        }

        // Zaktualizuj indeks modelu w MeshComponent
        comp->setIndex(newModelId);

        // Zaktualizuj mapê objectToGroup
        objectToGroup[obj] = group;

        std::cout << "Model ID changed from " << oldModelId << " to " << newModelId << " for group " << static_cast<int>(group) << ".\n";
    }

};
