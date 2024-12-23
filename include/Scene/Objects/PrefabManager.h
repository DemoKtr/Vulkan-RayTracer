#pragma once
#include <memory>
#include "Prefab.h"


namespace vkPrefab {
    class PrefabManager {
    public:
        static PrefabManager& getInstance() {
            static PrefabManager instance;
            return instance;
        }

        // Tworzy nowe okno, jeśli nie istnieje dla danego pliku
        std::shared_ptr<vkPrefab::Prefab> createPrefab( bool debugMode) {
            /*
            auto it = prefabs.find(filePath);
            if (it != prefabs.end()) {
                return it->second;
            }
            */
            auto prefab = std::make_shared<vkPrefab::Prefab>(debugMode);
            prefabs["ka"] = prefab;
            return prefab;
        }

        // Aktualizuje stan wszystkich okien i usuwa te zamknięte
        void update(float deltaTime) {
            for (auto it = prefabs.begin(); it != prefabs.end();) {
                if (it->second->isClosed()) {
                    it = prefabs.erase(it); // Usuwamy zamknięte okno z mapy
                }
                else {
                    it->second->render(deltaTime);
                    ++it;

                }
            }
        }

        // Zwraca liczbę aktywnych okien
        size_t getActiveWindowCount() const {
            return prefabs.size();
        }

    private:
        PrefabManager() = default;
        ~PrefabManager() = default;

        // Mapa plik -> obiekt okna
        std::unordered_map<std::string, std::shared_ptr<vkPrefab::Prefab>> prefabs;
    };
}
